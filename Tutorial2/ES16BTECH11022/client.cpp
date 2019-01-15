#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

#define PORT 3542

struct msgStruct {
	string name;
	string msg;
};

void readSocket(int socketID) {
	int count;

	while(1)
	{
		char buffer[100];

		if ((count = recv(socketID, buffer, sizeof(buffer), 0)) < 0)
		{
			close(socketID);
			break;
		}

		if (buffer[0] == '/')
		{
			cout<<"Too many members\n";
			close(socketID);
			exit(0);
		}

		cout<<buffer<<"\n";
	}
}

int main(int argc, char const *argv[])
{
	int sockid;
	struct sockaddr_in sockAddr;

	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	sockAddr.sin_port = htons(PORT);

	/*Socket Initialization*/
	sockid = socket(PF_INET, SOCK_STREAM, 0);

	if (sockid == -1)
	{
		cout<<"Error Creating Socket\n";
		return 1;
	}

	string name;

	cout<<"Enter Your Name: ";
	cin>>name;

	/*Connect to Server*/
	cout<<"Connecting...\n";

	while (connect(sockid, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0);	

	cout<<"Connected!\n\n";
	cout<<"End message with # to end chat!\n";
	cout<<"-------------------\n";

	thread readingThread = thread(readSocket, sockid);

	int count;
	msgStruct packet;
	packet.name = name;

	while(true) {
		string input;
		cin>>input;
		
		string msg;

		msg = name + ':' + input;


		// cout<<msg<<endl;
		// memcpy(&buffer, msg.c_str(), 100);

		// count = send(sockid, &packet, sizeof(packet), 0);
		if ((count = send(sockid, msg.c_str(), 100, 0)) < 0)
		{
			close(sockid);
			break;
		}
		if (input[input.size() - 1] == '#')
		{
			break;
		}
	}

	close(sockid);
	readingThread.detach();
	readingThread.~thread();
	return 0;
}