#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

#define PORT 3542

int main(int argc, char const *argv[])
{
	int server, client;
	struct sockaddr_in serverAddr, clientAddr;
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

	/*Binding the data in address with socket*/
	if (bind(server, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
	{
		cout<<"Bind Failed\n";
		return 1;
	}

	/*Set the socket into listen mode*/
	if (listen(server, 10) < 0)
	{
		cout<<"Listen Failed\n";
		return 1;
	}
	
	addrSize = sizeof(clientAddr);
	cout<<"Waiting For Client\n";

	client = accept(server, (struct sockaddr *) &clientAddr, &addrSize);

	cout<<"Connected!\n\n";
	cout<<"Enter Bye to end chat!\n";
	cout<<"-------------------\n";

	int count;
	while(true) {
		char buffer[50];
		count = recv(client, buffer, 50, 0);
		printf("Client: %s\n",buffer );
		if (strcmp(buffer,"Bye") == 0)
		{
			break;
		}

		char input[50];
		printf("You: ");
		scanf("%s",input);
		count = send(client, input, 50, 0);
		if (strcmp(input,"Bye") == 0)
		{
			break;
		}
	}

	close(client);
	close(server);
	return 0;
}