//Group B
//Lucas Stott
//lstott@okstate.edu
//3-3-2022
//Client Socket, Uses initial Fork() and Buffer to Communicate with Server



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5555
#define BUFSIZE 1024

int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr = {};

	clientSocket = socket(AF_INET, SOCK_STREAM, 0); // get socket type
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		return 1;
	}
	printf("[+]Client Socket is created.\n"); // generate address args to pass
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); // connection to server
	if(ret < 0){
		printf("[-]Error in connection.\n");
		return 1;
	}
	printf("[+]Connected to Server.\n");

	pid_t pid = fork(); // fork for message passing 
	if(pid < 0){ // if failed
		printf("[-]Error in forking.\n");
		return 1;
	}else if (pid > 0){ // if parents
		// listen for packets from server
		for (char buffer[BUFSIZE]; read(clientSocket, buffer, BUFSIZE) > 0;){ // what was passed from the server
			printf("\nReceived: %s\n", buffer);
		}
		printf("[-] Exiting listening fork"); // exiting
        return 1;
	}

	for (char buffer[BUFSIZE];;){ // listens to buffer
		// send whatever the client inputs
		// printf("Client: \t");
		fgets(buffer, BUFSIZE, stdin);
		sscanf(buffer, "%[^\n]\n", buffer); // formats buffer
		write(clientSocket, buffer, strlen(buffer) + 1); // writes to the socket
		printf("Sending: %s\n", buffer); // sending done 
		if(strcmp(buffer, ":exit") == 0){ // disconnect 
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			return 1;
		}
	}

	return 0;
}