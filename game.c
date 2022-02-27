//GroupB
//Joshua Hardin
//joshua.hardin@okstate.edu
//02/27/2022

#include "player.c"
#include "client.c"
//recvfrom chat_server the input data and the pid of the client who sent the input
void singleplayerGame(struct user plr){
    int s;
    server();//randomly selects one of the input files

    while(plr.skipCount < 3){//client always goes first in singleplayer
        client(plr);//will be changed to only print out the turn player menu
        while(true){//need to adjust this so that it only waits 4 minutes
            recvfrom(s, data, sizeof(data), 5555, sizeof(5555));//not sure if this is correct
            //that said, this more or less is to recieve data from chat_server.c
            recvfrom(s, pids, sizeof(pids), 5555, sizeof(5555));
            //we need a pid of the client who input the data
            if (pids == plr.clientpid){
                //send data to word processing to be checked if valid or not.
                //^needs to be added
                if(valid){
                    //update score, numFoundWords, numNewWords, currentWord, etc.
                    //^needs to be added
                    plr.skipCount = 0;
                }
                else{
                    //update score
                    //^needs to be added
                    plr.skipCount = plr.skipCount + 1;
                }
                break;
            }
        }
        if(plr.skipCount == 3){
            break;
        }
        gameOpponent(plr)//need to write gameOpponent
        if(valid){
            //update opponentScore, wordlist, etc.
            //^needs to be added
            plr.skipCount = 0;
        }
        else{
            //bot passed
            plr.skipCount = plr.skipCount + 1;
        }
    }
    scoreboard(plr);
}


void multiplayerGame(struct user plr[2]){
    int x;
    int y;
    int z;
    int s;
    server();
    x = 0;//needs to be replaced with a randomized method
    y = 1;

    while(plr[0].skipCount < 3){
        client(plr[x]);
        while(true){//top few ines are the same as in single player
            recvfrom(s, data, sizeof(data), 5555, sizeof(5555));
            recvfrom(s, pids, sizeof(pids), 5555, sizeof(5555));
            if(pids == plr[x].clientpid){
                //need to add lines to use word processing
                if(valid){
                    //coding needed
                    plr[0].skipCount = 0;
                }
                else{
                    //coding needed
                    plr[0].skipCount = plr[0].skipCount + 1;
                }
                break;
            }
        }
        z = x;
        x = y;
        y = z;
    }
    scoreboard(plr[0]);
    scoreboard(plr[1]);
}
