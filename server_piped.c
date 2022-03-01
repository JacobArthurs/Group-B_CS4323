#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Named Pipe Headers

#include <fcntl.h>
#include <wait.h>
#include <sys/stat.h>


#define PORT 5555
#define BUFSIZE 1024

int main(){

	int pipefd[2];
	if (pipe(pipefd) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    // Pipe Args
    // system("clear");

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		return 1;
	}
	printf("[+]Server Socket is created.\n");

	
	struct sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		return 1;
	}
	printf("[+]Bind to port %d\n", PORT);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
		return 1;
	}


	int playerSockets[1024];
	int newSocket;
	int playerID = 0;
	for (;;) {
		struct sockaddr_in newAddr = {};
		socklen_t addr_size;
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			return 1;
		}
		playerSockets[playerID++] = newSocket;
		char msg[BUFSIZE];
		sprintf(msg, "CONNECT\n%i", newSocket);
		write(pipefd[1], msg, strlen(msg) + 1);
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		pid_t pid = fork();
		if(pid < 0){
			printf("[-]Error in forking.\n");
			return 1;
		}else if (pid > 0){
			write(pipefd[1], "EXIT", strlen("EXIT") + 1);
			pid_t pid1 = fork();
			if (pid1 < 0){
				printf("[-]Error in forking.\n");
				return 1;
			}else if (pid1 == 0){
				printf("pipe child: %d -- master parent: %d", getpid(), getppid());
				
				char buffer[BUFSIZE]; 
				while (read(pipefd[0], buffer, BUFSIZE) > 0){
					printf("\nCOMMAND:%s\n", buffer);
					char command[BUFSIZE];
					sscanf(buffer, "%[^\n]\n", command);
					if (strcmp(command, "MSG") == 0) {
						puts("Executing MSG CODE:");
						// forward MSG from one player to all players
						for (int i = 0; i < playerID; ++i){
							printf("BUFFER:%sPlayerSocket%i\n", buffer, playerSockets[i]);
							write(playerSockets[i], buffer, strlen(buffer) + 1);
						}
					}else if (strcmp(command, "EXIT") == 0){
						return 0;
					}
				}
				return 0;
			}
			continue;
		}else{
			break;
		}
	}
	
	// child code
	printf("socket child: %d -- master parent: %d", getpid(), getppid());
	for (;;){
		char buffer[BUFSIZE];
    	ssize_t numBytesRcvd = recv(newSocket, buffer, BUFSIZE, 0);
		char msg[BUFSIZE];
		sprintf(msg, "MSG\n%i\n%s", playerID, buffer);
		write(pipefd[1], msg, strlen(msg) + 1);
	}

	// close(sockfd);
	// close(newSocket);


	return 0;
}
