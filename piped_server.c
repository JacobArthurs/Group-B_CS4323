//Group B
//Lucas Stott
//lstott@okstate.edu
//3-3-2022
//Server Socket, Two Fork()'s to handle a single PipeFd to handle POSIX message passing. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Named Pipe Headers (NOT USED SINCE INPLEMENTED 2 FORKS FOR EACH UNIQUE PIPE)

#include <fcntl.h>
#include <wait.h>
#include <sys/stat.h>


#define PORT 5555
#define BUFSIZE 1024

int main(){

	int pipefd[2]; //pipe initialized 
	if (pipe(pipefd) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

	int sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket for server
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		return 1;
	}
	printf("[+]Server Socket is created.\n");

	
	struct sockaddr_in serverAddr = {}; // setup address for socket, IP, PORT, TYPE OF SOCKET
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){ // connection test
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


	int playerSockets[1024]; // player socket setup can hold up to 1024 players (IDK WHY 1024 but good number) should be 6 for 3 players
	int newSocket; // socket FD that gets passed to the forks
	int playerID = 0; // start with player 0 then incriment as players join server

	for (;;) { // infinite loop until server exits
		struct sockaddr_in newAddr = {};
		socklen_t addr_size;
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			return 1;
		}
		playerSockets[playerID++] = newSocket; // set playerSocket for each player
		char msg[BUFSIZE];
		sprintf(msg, "CONNECT\n%i", newSocket); // concated message
		write(pipefd[1], msg, strlen(msg) + 1); // write to pipe the message [1] 
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		pid_t pid = fork(); // first fork
		if(pid < 0){ // check if failed
			printf("[-]Error in forking.\n");
			return 1;
		}else if (pid > 0){ // parent process
			write(pipefd[1], "EXIT", strlen("EXIT") + 1); // write to parent
			pid_t pid1 = fork(); // second fork
			if (pid1 < 0){ // check if failed
				printf("[-]Error in forking.\n");
				return 1;
			}else if (pid1 == 0){ // if child process
				//printf("Pipe Child: %d -- Master Parekt: %d\n", getpid(), getppid());
				char buffer[BUFSIZE]; 
				while (read(pipefd[0], buffer, BUFSIZE) > 0){ // if pipefd[0] gets input 
					printf("\nCOMMAND:%s\n", buffer); // print the COMMAND passed
					char command[BUFSIZE];
					sscanf(buffer, "%[^\n]\n", command); // just pretty up output
					if (strcmp(command, "MSG") == 0) { // what the message is
						puts("Executing MSG CODE"); // shows that the message was successfully outputed
						// forward MSG from one player to all players
						for (int i = 0; i < playerID; ++i){
							//printf("PlayerSocket[%i] Sent:\nBUFFER:\n%s", playerSockets[i], buffer);
							write(playerSockets[i], buffer, strlen(buffer) + 1); // write to the player
						}
					}else if (strcmp(command, "EXIT") == 0){ // if comment is EXIT then exit all
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
	
	// Child Code
	//printf("Pipe Child: %d -- Master Parekt: %d\n", getpid(), getppid());
	for (;;){
		char buffer[BUFSIZE];
    	ssize_t numBytesRcvd = recv(newSocket, buffer, BUFSIZE, 0); // grab whats in the buffer after push
		char msg[BUFSIZE];
		sprintf(msg, "MSG\nPlayer ID[%i]\nMessage:%s", playerID, buffer); // concate message
		write(pipefd[1], msg, strlen(msg) + 1); // send through pipe
	}

	// close(sockfd);
	// close(newSocket);

	return 0;
}
