//Group B
//Lucas Stott
//lstott@okstate.edu
//2-27-2022
//Client Socket, Uses Fork for Recieving and Sending 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>

#define MAXMSG 1024 // Size of Buffer 
// Use Port 5555 For Server
// Use IP 127.0.0.1
// ./client 127.0.0.1 5555


int main (int argc, char const *argv[])
{

    int port, get_addr, sd, read_listen, send_listen; // port = Port, get_addr = Address, sd = socket desciptor, rval & sval = listeners for buffer 
    struct addrinfo hints, *result, *rp; // Address Descriptors 


    if (argc != 3) 
    {
        fprintf(stderr, "Not Enough Args, Use:\n./Cliend ID PORT\n");
        exit(1);
    }

    // Data Structure for Stream IPC SOCKET
    memset (&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    // Hostname, Ports, Hints, Arguments
    get_addr = getaddrinfo(argv[1], argv[2], &hints, &result);

    if (get_addr != 0) 
    {
        fprintf(stderr, "Get Address Info Failed: %s\n", gai_strerror(get_addr));
        exit(EXIT_FAILURE); // Output Error
    }
    
    // Dynamically Search for Port to Connect Socket
    // Breaks on Success
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sd == -1)
            continue;

        if (connect(sd, rp->ai_addr, rp->ai_addrlen) != -1)
            break; // Connection Successful

        close(sd);
    }

    if (rp == NULL)
    {
        fprintf(stderr, "Socket Address Invalid.\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    int read_buffer[MAXMSG];       // Read Buffer
    int send_buffer[MAXMSG];       // Send Buffer

    int pid = fork();                     // Forks to Split the Send and Receive
    send_listen = read_listen = 1;        // While True Statement (1 = true)
    while (send_listen && read_listen)    // While Both Are True
    {   
        if (pid == 0)   // Reads for Socket (Child)
        {
            if ( (read_listen = read(sd , &read_buffer , sizeof(read_buffer))) ) 
            {
                read_buffer[read_listen] = '\0'; // Null Terminator 
                write(1, &read_buffer, read_listen);
            }
        }else{          // Writes to Socket (Parent)
            if ( (send_listen = read(0 , &send_buffer , sizeof(send_buffer))) ) 
            {   
                send_buffer[send_listen] = '\0'; // Null Terminator 
                send(sd, &send_buffer, send_listen,0);
            }
        }
    }
    puts("Exiting the Game");
    if (pid == 0) exit(0);  // Exits Child
    kill(pid,SIGKILL);      // Kill Child Process
    wait(NULL);             // Waits So Zombie Process does not Occer
    shutdown(sd,2);         // Shuts down Socket
    close(sd);              // Close

    return 0;
}