#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <errno.h>
#include <string.h>   
#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 
  
#define TRUE   1
#define FALSE  0
#define PORT 5555 
#define MAXMSG 1024 //size of buffer 
#define MAXCLIENTS 3

int main(int argc , char *argv[])
{
    int opt = TRUE;
    int sd_is_init = FALSE;
    int ms; // Master Socket with all Clients
    int addrlen , new_socket, sd; // socket and length var
    int client_socket_init[MAXCLIENTS]; // sockets that have clients listening
    int client_socket[MAXCLIENTS];  // main sockets, set to 0 for holder
    int activity, valread; // activity (checks for buffer input) , valread (check buffer size)
    int max_clients = 3; // max client connetions
    int i, j, k; // loop vars
    int max_sd; // max discriptor list
    char *message;  // pointer to message to send to client
    struct sockaddr_in address; // server address
      
    char buffer[MAXMSG]; // Main Buffer
    char mes[MAXMSG]; // Seconday Buffer 
    
    fd_set readfds;
    
    for (i = 0; i < max_clients; i++)                                           // Default Client Sockets to 0
    {
        client_socket[i] = 0;
    }
      
    if( (ms = socket(AF_INET , SOCK_STREAM , 0)) == 0)                          // Master Socket Creation 
    {
        perror("Master Socket Failed: ");
        exit(EXIT_FAILURE);
    }

    if( setsockopt(ms, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) // Master Socket to Accept Mult Clients
    {
        perror("Multiple Socket Creation Failed: ");
        exit(EXIT_FAILURE);
    }
   
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;                                       // Get Any Address (127.0.0.1) = Local
    address.sin_port = htons(PORT);                                             // Port Set to 5555
      
    if ( bind(ms, (struct sockaddr *)&address, sizeof(address)) < 0 )           // Bind Master Socket to Address 
    {
        perror("Binding has Failed: ");
        exit(EXIT_FAILURE);
    }

    printf("Listening on Port %d \n", PORT);
     
    if (listen(ms, MAXCLIENTS) < 0)                                             // Only Listening for MaxClients
    {
        perror("Listening has Failed: ");
        exit(EXIT_FAILURE);
    }
      
    addrlen = sizeof(address);                                                  // Size of Struct address
    puts("---------------Server waiting for Clients---------------");
     
    while(TRUE) 
    {
        FD_ZERO(&readfds);                                                      // Clear File Descipters 
        FD_SET(ms, &readfds);                                                   // Add File Descipters to Master Socket
        max_sd = ms;                                                            // Number of File Desciptors   
        
        for (i = 0 ; i < max_clients ; i++)                                     // Loop to add Clients to Socket Descipter 
        {
            sd = client_socket[i];
            if( sd > 0 )                                                        // If Client Socket Exist then Add to Socket Descripter 
                FD_SET(sd , &readfds);
            if(sd > max_sd)                                                     // Set New Number Higher of Descriptors 
                max_sd = sd;
        }
        // Waits for Change on Desciptors 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR))                                   // Error Occured on Select
        {
            perror("Select Error: ");
        }
          
       
        if (FD_ISSET(ms, &readfds))                                             // Accepting New Sockets 
        {
            if ((new_socket = accept(ms, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("Accept Failed: ");
                exit(EXIT_FAILURE);
            }
            /* Prints Details About Client Connecting to Server*/
            printf("New Connection\nSocket Discriptor : %d\nIP : %s\nPort : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
            
            message = "Session started! \n\0";
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("Sending Fialed: ");
            }                                    

            for (i = 0; i < max_clients; i++)                                       // Adding new Socket to Initializer 
            {
                if( client_socket[i] == 0 )                                         // Gets Empty Position
                {
                    client_socket[i] = new_socket;
                    client_socket_init[i] = new_socket;
                    printf("Added New Client To Position: %d\n" , i);               // Print Client Position
                    break;
                }
            }
        } // we need to add something to recieve each clients first input, call player.c so that they fill out their name and 
        // country, and a way to send them to either singleplayer or multiplayer game
        //struct user newPlr

        int fork1 = fork();
        if(fork1 > 0){
            // we need the process that keeps track of all the input data and the client who did here
            //pipeline the data over
        }
        if(fork1 == 0){
            //pipelined data recieved here
            //we need the input from where new clients are added, add it to player.c struct

            int fork2 = fork();

            if (fork2 > 0){
                if(newPlr.gametype == 1){
                    server();//randomly selects one of the input files
                    struct wordList validWords;
                    find_valid_words();//probably want to use the plr.randomAlphabets to be the valid words and set validWords
                    //to be the list of valid words
                    while(plr.skipCount < 3){//client always goes first in singleplayer
                        client(plr);//will be changed to only print out the turn player menu
                        while(true){//need to adjust this so that it only waits 4 minutes
                            //pipleined data is looked at here
                            //we need a pid of the client who input the data
                            if (pids == plr.clientpid){
                                if (plr.validWords == 3){
                                    plr.skipCount = plr.skipCount + 1;
                                    plr.validWords = 0;
                                    break;
                                }
                                else if(/*pass condition*/){
                                    plr.skipCount = plr.skipCount + 1;
                                    plr.validWords = 0;
                                    break;
                                }
                                else if(is_word_valid()){
                                    register_points();
                                    register_word();
                                    plr.currentWord = ;//input
                                    plr.skipCount = 0;
                                    plr.validWords = 0;
                                }
                                else{
                                    register_points();
                                    plr.validWords = plr.validWords + 1;
                                }
                            }
                        }
                        if(plr.skipCount == 3){
                            break;
                        }
                        gameOpponent(plr)//need to write gameOpponent
                        if(valid){
                            register_points();
                            register_word();
                            plr.currentWord = ;//server answer
                            plr.skipCount = 0;
                        }
                        else{//bot passed
                            plr.skipCount = plr.skipCount + 1;
                        }
                    }
                    scoreboard(plr);
                }
            }
        }
        
        for (i = 0; i < max_clients; i++)                                           // Main Program 
        {   
            sd = client_socket[i];                                                  // Socket Discriptor
            if (FD_ISSET(sd, &readfds))                                             // Waiting for Response 
            {
                for (k = 0; k < max_clients; k++)                                   // Check for Discriptor
                {
                    if(sd == client_socket_init[k]) {                               // Set Client to Send to Server 
                        break;
                    }
                }

                if ((valread = read(sd , buffer, MAXMSG)) == 0) // Reads Buffer from Client if 0 then Player Disconnected 
                { 
                    /* Gets Peer Socket and Address Details */
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen); 
                    printf("Host disconnected , ip : %s , port : %d , port : %d\n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port) , i);
                    shutdown(sd,2);                                                     // Shutdown Socket
                    close(sd);                                                          // Close Socket
                    client_socket[i] = 0;                                               // Set Socket to 0 for Open 
                    if(sd_is_init) client_socket_init[i] = 0;                           // Remove from List
                    printf("Player Has Left\n");
                }
                else // This is Where the Buffer is Handles and Messages are Sent
                { 
                    memset(mes, 0, strlen(mes));                                        // Reset mes String Empty
                    strcat(mes, buffer);                                                // Take Buffer into mes
                    for (k = 0; k < max_clients; k++)                                   // Interate Through Clients 
                    {   
                        if (k == i) {                                                   // Skip is self
                            continue;
                        } else {
                            //printf("%s",mes);                                         // String in mes (Will Print For Each Max Length)
                            send(client_socket_init[k], mes, strlen(mes), 0);           // Send to Each Client that is Not Self
                        }
                    }
                    memset(buffer, 0, strlen(buffer));                                  // Erase Buffer for Next Message
                }
            }
        }
    }
    close(ms);                                                                          // Close Master Socket
    return 0;
} 