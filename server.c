#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h> 

int main(int argc, char const *argv[]){

    int server_sk, new_sk, client_read, port, retval, error;
    int opt = 1;
    pid_t pid, pidNew;
    char buffer[4096] = {0};
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int addrlen = sizeof(server_addr);
    int addrlen_c = sizeof(client_addr);

    char* welcome = "Welcome to the server! \n";

    if(argc <= 1){
        printf("Too few arguments");
    }else{
        port = atoi(argv[1]);
    }

    //Creating the socket using the socket() function
    if((server_sk = socket(AF_INET, SOCK_STREAM,0)) == 0 ){
        perror("\n Socket could not connect \n");
        exit(EXIT_FAILURE);
    }

    //Attaching the socket to the port known to the client
    if( setsockopt(server_sk, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
        perror("Socket could not attach to port");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    //Attaching the socket to the port known to the client
    if(bind(server_sk, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        perror("Socket could not bind to the port");
        exit(EXIT_FAILURE);
    }

    //Starts listening for connections
    if(listen(server_sk, 3) < 0){
        perror("Could not listen");
        exit(EXIT_FAILURE);
    }

    //While loop to look for mutliple client connections
    while(1){

        //Connects Client
        if((new_sk = accept(server_sk, (struct sockaddr *) &server_addr, (socklen_t *) &addrlen_c)) < 0){
            perror("Could not accept new server");
            exit(EXIT_FAILURE);
        }

        pid = fork();

        if(pid < 0){
            close(new_sk);
            printf("Could not process fork\n");
            continue;
        }
        //Child fork for a single client
        else if(pid == 0){

            //While loop to manage multiple commands from one client
            while(1){

                //Emptying buffer to avoid errors
                bzero(buffer,sizeof(buffer));

                //Reading command from client
                client_read = read(new_sk, buffer, 1024);

                //Closes connection and exists process if command if quit
                if(strcmp("quit", buffer) == 0){
                    printf("Terminating Client!\n");
                    close(new_sk);
                    exit(EXIT_FAILURE);
                    break;
                }

                //Code to form args
                int counter = 1;
                char** args = malloc(sizeof(char*)*1024);
                char * command;

                command = strtok(buffer, " ");
                args[0] = command;

                while((command = strtok(NULL, " ")) != NULL){
                    args[counter] = command;
                    counter++;
                }

                args[counter] = NULL;
                
                pidNew = fork();

                if(pidNew < 0){
                    printf("Could not process fork\n");
                    continue;
                }
                //Child process for single command
                else if(pidNew == 0){
                    dup2(new_sk, STDOUT_FILENO);
                    dup2(new_sk, STDERR_FILENO);       
                    if((error = execvp(args[0], args)) == -1) {
				        perror("Could not execute command.\n");
                        exit(EXIT_FAILURE);
		            }
                }
                //Parent process for second fork
                else{
                    wait(NULL);
                }
            }
            exit(1);
        }
        //Parent process for first fork
        else{
            close(new_sk);
        }
    }
    return 0;
}

