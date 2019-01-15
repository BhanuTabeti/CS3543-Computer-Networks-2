#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstdbool>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

#define PORT 3542

map<int,bool> clients;
vector<thread> readingThreads;

struct msgStruct {
	string name;
	string msg;
};

int N;

void readSocket(int socketID) {
	int count;
	struct msgStruct packet;

	while(1)
	{
		char buffer[100];

		if ((count = recv(socketID, buffer, sizeof(buffer), 0)) < 0)
		{
			map<int,bool>::iterator it = clients.find(socketID);
			clients.erase(it);
			close(socketID);
			return;
		}

		cout<<buffer<<endl;
		for (std::map<int,bool>::iterator i = clients.begin(); i != clients.end(); ++i)
		{
			if ((*i).first == socketID)
			{
				continue;
			}
			else{
				if ((count = send((*i).first, buffer, sizeof(buffer), 0)) < 0)
				{
					clients.erase(i);	
				}
			}
		}

		int len = strlen(buffer);
		if (buffer[len-1] == '#')
		{
			map<int,bool>::iterator it = clients.find(socketID);
			clients.erase(it);
			close(socketID);
			return;
		}
	}
}

void acceptingClients(int socketID) {
	while (1)
	{
		struct sockaddr_in clientAddr;
		socklen_t addrSize = sizeof(clientAddr);
		int client;

		if (client = accept(socketID, (struct sockaddr *) &clientAddr, &addrSize))
		{
			if (clients.size() >= N)
			{
				string msg = "/";
				send(client, msg.c_str(), 100, 0);
				close(client);
			}
			else
			{
				clients[client] = true;
				readingThreads.push_back(thread(readSocket, client));				
			}
		}
		else{
			break;
		}
	}
}


int main(int argc, char const *argv[])
{
	int server,ok;
	struct sockaddr_in serverAddr;
	socklen_t addrSize;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

	/*Socket Initialization*/
	server = socket(AF_INET, SOCK_STREAM, 0);

	if (server == -1)
	{
		cout<<"Error Creating Socket\n";
		return 1;
	}

	if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &ok, sizeof(ok)))
	{
		perror("Setsockopt Failed");
		exit(EXIT_FAILURE);
	}

	/*Binding the data in address with socket*/
	if (bind(server, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
	{
		cout<<"Bind Failed\n";
		return 1;
	}

	// Maximum No. of Clients
	
	cout<<"Enter Maximum No. of Clients: ";
	cin>>N;

	/*Set the socket into listen mode*/
	if (listen(server, 100) < 0)
	{
		cout<<"Listen Failed\n";
		return 1;
	}
	
	cout<<"Accepting Clients\n";
	thread acceptingThread = thread(acceptingClients,server);

	char dum;
	cin>>dum;

	for (int i = 0; i < readingThreads.size(); ++i)
		readingThreads[i].join();
	
	close(server);
	acceptingThread.detach();
	acceptingThread.~thread();

	return 0;
}