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

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		return 1;
	}
	printf("[+]Client Socket is created.\n");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		return 1;
	}
	printf("[+]Connected to Server.\n");

	pid_t pid = fork();
	if(pid < 0){
		printf("[-]Error in forking.\n");
		return 1;
	}else if (pid > 0){
		// listen for packets from server
		for (char buffer[BUFSIZE]; read(clientSocket, buffer, BUFSIZE) > 0;){
			printf("\nReceived: %s\n", buffer);
		}
		printf("[-] Exiting listening fork");
        return 1;
	}

	for (char buffer[BUFSIZE];;){
		// send whatever the client inputs
		printf("Client: \t");
		fgets(buffer, BUFSIZE, stdin);
		sscanf(buffer, "%[^\n]\n", buffer);
		write(clientSocket, buffer, strlen(buffer) + 1);
		printf("Sending: %s\n", buffer);
		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			return 1;
		}
	}

	return 0;
}