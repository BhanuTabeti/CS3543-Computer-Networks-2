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

	/*Connect to Server*/
	cout<<"Connecting...\n";

	while (connect(sockid, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0);	

	cout<<"Connected!\n\n";
	cout<<"Enter Bye to end chat!\n";
	cout<<"-------------------\n";

	int count;
	while(true) {
		char input[50];
		printf("You: ");
		scanf("%s",input);
		count = send(sockid, input, 50, 0);
		if (strcmp(input,"Bye") == 0)
		{
			break;
		}

		char buffer[50];
		count = recv(sockid, buffer, 50, 0);
		printf("Client: %s\n",buffer );
		if (strcmp(buffer,"Bye") == 0)
		{
			break;
		}

	}

	close(sockid);
	return 0;
}