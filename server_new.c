//Group B
//Lucas Stott
//lstott@okstate.edu
//2-27-2022
//Server Socket, Services 3 Players. Forks Game for User, Does not Fork Socket but resets Socket dynamically  

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
#include "player.c"
#include "word_processing.c"
#include "server.c"
#include "scoreboard.c"

  
#define TRUE   1
#define FALSE  0
#define PORT 5555  // Port Number
#define MAXMSG 1024 // Maximum Size of Buffer
#define MAXCLIENTS 3 // Maximum Number of Clients

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
    int multi[2] = {99, 99}; // 99 = Null
    int single = 99;
    int first_player;
    int second_player;
    int single_player;
    struct user plr[3];
    struct wordList validWords[3];
    
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
            
            message = "Session started! \n1. Single Player Mode\n2. Multi-Player Mode\n3. Exit\nPlease Select Options: \0";
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("Sending Fialed: ");
            }           
                     
            puts("---------------New Player Has Joined---------------");
            for (i = 0; i < max_clients; i++)                                       // Adding new Socket to Initializer 
            {
                if( client_socket[i] == 0 )                                         // Gets Empty Position
                {
                    client_socket[i] = new_socket;
                    client_socket_init[i] = new_socket;
                    break;
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
                    printf("Host Disconnected , IP : %s , PORT : %d , PORT : %d\n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port) , i);
                    shutdown(sd,2);                                 // Shutdown Socket
                    close(sd);                                      // Close Socket
                    client_socket[i] = 0;                           // Set Socket to 0 for Open 
                    if(sd_is_init) client_socket_init[i] = 0;       // Remove from List
                    if (multi[0] == i || multi[1] == i) {           // Reset the Multi Listener
                        if (multi[0] == i) {
                            multi[0] = 99;
                        } else {
                            multi[1] = 99;
                        }
                        puts("Reset Multi-Player Port");
                    }

                    if (single = i) {
                        single = 99;
                        puts("Reset Single Player Port");
                        break;
                    }
                    
                    printf("Player[%i] Has Left\n", i);
                    puts("--------------------Player Left--------------------");
                }
                else // This is Where the Buffer is Handles and Messages are Sent
                { 
                    if (multi[0] == i || multi[1] == i) { // If in Multi-Port dont Read from Here
                        break;
                    }
                    if (single == i) {
                        break;
                    }

                    memset(mes, 0, strlen(mes));    // Reset mes string
                    strcat(mes, buffer);            // Reads buffer into mes
                    printf("Player[%i] -> typed: %s", i, mes);

                    if ((strncmp(mes, "1\n", strlen(mes)) == 0)) { //Entering Single Player Game
                        while(1){ // Keep State (Don't Really Need)
                            printf("Player[%i]: Entering Single Player Game\n", i);
                            printf("Child[%d] Parent[%d]\n", getpid(), getppid());
                            int player;
                            if(plr[0].clientpid == 0){
                                plr[0].clientpid = client_socket_init[i];
                                player = 0;
                            }
                            else if(plr[1].clientpid == 0){
                                plr[1].clientpid = client_socket_init[i];
                                player = 1;
                            }
                            else{
                                plr[2].clientpid = client_socket_init[i];
                                player = 2;
                            }
                            message = "Entering Single Player Game\n\0";
                            memset(mes, 0, strlen(mes));                                        
                            strcat(mes, message); 
                            send(client_socket_init[i], mes, strlen(mes), 0);
                            if (single == 99) {
                                single = i;
                                single_player = single;
                            }
                            char *singlegame_message = "SENT WITH Single Player Port\n\0"; // Message Pass to Both Players
                           // plr[player] = malloc(1000);
                            send(client_socket_init[single_player], singlegame_message, strlen(singlegame_message), 0);
                            fname(plr[player].clientpid);                       //ask the player to fill in their name
                            recv(plr[player].clientpid,buffer,1024,0);          //and country
                            strcpy(plr[player].firstName,buffer);
                            lname(plr[player].clientpid);
                            recv(plr[player].clientpid,buffer,1024,0);
                            strcpy(plr[player].lastName,buffer);
                            cnty(plr[player].clientpid);
                            recv(plr[player].clientpid,buffer,1024,0);
                            strcpy(plr[player].country,buffer);
                            server(&plr[player], &validWords[player]);   //randomly select a input file
                            while(plr[player].skipCount < 3){//client always goes first in singleplayer
                                if(plr[player].skipCount == 2){//check if the current word was cleared so that
                                    strcpy(plr[player].currentWord, "");
                                }
                                print_game_status(plr[player]);//ask about later
                                char input[20];
                                while(true){//client turn
                                    if (plr[player].validWords == 3){//skip turn
                                        plr[player].skipCount = plr[player].skipCount + 1;
                                        plr[player].validWords = 0;
                                        strcpy(input, plr[player].currentWord);
                                        break;
                                    }
                                    recv(plr[player].clientpid,buffer,1024,0);//ask for input
                                    strcpy(input, buffer);
                                    if(strncmp(input, "Pass", strlen(input)) == 0){//pass from client
                                        plr[player].skipCount = plr[player].skipCount + 1;
                                        plr[player].validWords = 0;
                                        strcpy(input, plr[player].currentWord);
                                        break;
                                    } else if(is_word_valid(input, plr[player].randomAlphabets, plr[player],
                                            &validWords[player], false, plr[player].currentWord[strlen(plr[player].currentWord)-1])){//valid word inputted
                                        register_points(plr[player], input, &validWords[player]);
                                        register_word(input,&plr[player]);
                                        strcpy(plr[player].currentWord, input);//input
                                        plr[player].skipCount = 0;
                                        plr[player].validWords = 0;
                                        break;
                                    } else if(has_been_used(input, plr[player])){//inputted used word
                                        strcpy(input, "used");
                                        register_points(plr[player], input, &validWords[player]);
                                        plr[player].validWords = plr[player].validWords + 1;
                                        strcpy(input, plr[player].currentWord);
                                    } else{//inputted invalid word
                                        strcpy(input, "invalid");
                                        register_points(plr[player], input, &validWords[player]);
                                        plr[player].validWords = plr[player].validWords + 1;
                                        strcpy(input, plr[player].currentWord);
                                    }
                                }
                                if(plr[player].skipCount == 3){//end game
                                    break;
                                } else if(plr[player].skipCount == 2){//clear current word
                                    strcpy(plr[player].currentWord, "");
                                    strcpy(input, "");
                                }
                                generate_oppponent_word(plr[player], plr[player].currentWord, &validWords[player]);//server opponent picks word
                                
                                if((strncmp(plr[player].currentWord, input, strlen(input)) == 0)){//if server picked a word
                                    register_points(plr[player], plr[player].currentWord, &validWords[player]);
                                    register_word(plr[player].currentWord, &plr[player]);
                                    plr[player].skipCount = 0;
                                } else{//bot passed
                                    plr[player].skipCount = plr[player].skipCount + 1;
                                }
                            }
                            singlePlayerScoreboard(plr[player]);//print scoreboard
                            break;
                        }
                    }

                    if ((strncmp(mes, "2\n", strlen(mes)) == 0)) { //Entering Multi Player Game
                        while(1){
                            printf("Player[%i]: Entering Multi-Player Player Game\n", i);
                            printf("Child[%d] Parent[%d]\n", getpid(), getppid());
                            int x;
                            int y;
                            int z;
                            message = "Entering Multi-Player Game\n\0";
                            memset(mes, 0, strlen(mes));                                        
                            strcat(mes, message);
                            send(client_socket_init[i], mes, strlen(mes), 0);
                            if (multi[0] == 99 && multi[1] == 99) { //Checks is both multi-port is Open
                                multi[0] = i;
                                first_player = multi[0]; //Sets First Player port
                                printf("First Player on multi[%i]\n",i);
                            } else { // If One Port is Taken Do This
                                if (multi[0] != 99) { // Check First Port is Open
                                    printf("SOCKET CONNECTER[%i]\n", i);
                                    multi[1] = i;
                                    second_player = multi[1];
                                    printf("second_player[%i], multi[%i]", second_player, multi[1]);
                                    //printf("Multi[0]:%i, Multi[1]:%i", multi[0], multi[1]);
                                    puts("Added Second Player to multi[1]\n");
                                }
                                else { // Else Attach To First Port
                                    multi[0] *= i;
                                    second_player = multi[0];
                                    printf("second_player[%i]\n", second_player);
                                    //printf("Multi[0]:%i, Multi[1]:%i", multi[0], multi[1]);
                                    puts("Added Second Player to multi[0]\n");
                                }
                                puts("SECOND PLAYER ARRIVED"); // State Of Game
                                printf("FirstPlayer[%i] - SecondPlayer[%i]\n", first_player, second_player);
                                char *test_message = "SENT WITH first_player & second_player\n\0"; // Message Pass to Both Players
                                send(client_socket_init[first_player], test_message, strlen(test_message), 0); // Send to Player One
                                send(client_socket_init[second_player], test_message, strlen(test_message), 0); // Send to Player Two
                                // Starting Game Here
                                x = 1;
                                y = 2;
                                plr[1].clientpid = client_socket_init[first_player];
                                plr[2].clientpid = client_socket_init[second_player];
                                fname(plr[x].clientpid);                    //ask the two players to input their
                                recv(plr[x].clientpid,buffer,1024,0);       //name and their country
                                strcpy(plr[x].firstName,buffer);
                                lname(plr[x].clientpid);
                                recv(plr[x].clientpid,buffer,1024,0);
                                strcpy(plr[x].lastName,buffer);
                                cnty(plr[x].clientpid);
                                recv(plr[x].clientpid,buffer,1024,0);
                                strcpy(plr[x].country,buffer);
                                fname(plr[y].clientpid);
                                recv(plr[y].clientpid,buffer,1024,0);
                                strcpy(plr[y].firstName,buffer);
                                lname(plr[y].clientpid);
                                recv(plr[y].clientpid,buffer,1024,0);
                                strcpy(plr[y].lastName,buffer);
                                cnty(plr[y].clientpid);
                                recv(plr[y].clientpid,buffer,1024,0);
                                strcpy(plr[y].country,buffer);
                                server(&plr[x], &validWords[x]);
                                strcpy(plr[y].randomAlphabets, plr[x].randomAlphabets);
                                strcpy(plr[y].currentWord, plr[x].currentWord);
                                while(plr[1].skipCount < 3){//client always goes first in singleplayer
                                    if(plr[1].skipCount == 2){
                                        strcpy(plr[x].currentWord, "");
                                    }
                                    print_game_status(plr[x]);//ask about later
                                    char input[20];
                                    while(true){ //players turn
                                        if (plr[x].validWords == 3){//end of turn due to too many invalid answers
                                            plr[1].skipCount = plr[1].skipCount + 1;
                                            plr[x].validWords = 0;
                                            break;
                                        }
                                        recv(plr[x].clientpid,buffer,1024,0);//recieve input from turn player
                                        strcpy(input, buffer);
                                        if(strncmp(input, "Pass", strlen(input)) == 0){//pass condition
                                            plr[1].skipCount = plr[1].skipCount + 1;
                                            plr[x].validWords = 0;
                                            break;
                                        } else if(is_word_valid(input, plr[x].randomAlphabets, plr[x],
                                                &validWords[1], false, plr[x].currentWord[strlen(plr[x].currentWord)-1])){//valid answer condition
                                            register_points(plr[x], input, &validWords[1]);
                                            plr[y].opponentScore = plr[x].score;
                                            register_word(input,&plr[x]);
                                            register_word(input,&plr[y]);
                                            strcpy(plr[x].currentWord, input);//input
                                            strcpy(plr[y].currentWord, plr[x].currentWord);
                                            plr[1].skipCount = 0;
                                            plr[x].validWords = 0;
                                            break;
                                        } else if(has_been_used(input, plr[x])){// used input condition
                                            strcpy(input, "used");
                                            register_points(plr[x], input, &validWords[1]);
                                            plr[y].opponentScore = plr[x].score;
                                            plr[x].validWords = plr[x].validWords + 1;
                                        } else{//invalid input condition
                                            strcpy(input, "invalid");
                                            register_points(plr[x], input, &validWords[x]);
                                            plr[y].opponentScore = plr[x].score;
                                            plr[x].validWords = plr[x].validWords + 1;
                                        }
                                    }
                                    z = x;
                                    x = y;
                                    y = z;  
                                }
                            }
                            multiplayerScoreboard(plr[1]);//print out the game
                            multiplayerScoreboard(plr[2]);
                            break;
                        }
                    }
                    
                    if (!(strncmp(mes, "3\n", strlen(mes)) == 0)) { // Exit Game
                        printf("Player[%i]: Exiting\n", i);
                        printf("Child[%d] Parent[%d]\n", getpid(), getppid());
                        getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen); 
                        printf("Host Disconnected , IP: %s , PORT: %d , PORT: %d\n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port) , i);
                        puts("--------------------Player Left--------------------");
                        shutdown(sd,2);                                                     // Shutdown Socket
                        close(sd);                                                          // Close Socket
                        client_socket[i] = 0;                                               // Set Socket to 0 for Open 
                        if(sd_is_init) client_socket_init[i] = 0;                           // Remove from List
                        break;
                    }

                    // send(sd, mes, strlen(mes), 0);                                   // Send to Self
                    /* SENDS TO ALL CLIENTS */
                    // for (k = 0; k < max_clients; k++)                                   // Interate Through Clients 
                    // {   
                    //     if (k == i) {                                                   // Skip is self
                    //         continue;
                    //     } else {
                    //         // printf("%s",mes);                                        // String in mes (Will Print For Each Max Length)
                            
                    //         send(client_socket_init[k], buffer, strlen(buffer), 0);     // Send to Each Client that is Not Self
                    //     }
                    // }
                    memset(buffer, 0, strlen(buffer));                                  // Erase Buffer for Next Message
                }
            }
        }
    }
    close(ms);                                                                          // Close Master Socket
    return 0;
} 
