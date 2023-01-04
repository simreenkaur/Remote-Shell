#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 7891

int main(int argc, char const *argv[]){

    int sk, port, server_read, counter;
    char buffer[1024] = {0};
    char * host;
    struct sockaddr_in server_addr;
    struct hostent * host_pointer;
    struct protoent * protocol_pointer;

    if(argc < 3){
        printf("Too few arguments");
        return -1;
    }
   
    host = argv[1];
    port = atoi(argv[2]);
    host_pointer = gethostbyname(host);

    if((char *) host == NULL){
        printf("Could not recognize host name");
        return -1;
    }

    memcpy(&server_addr.sin_addr, host_pointer->h_addr_list[0], host_pointer->h_length);

    //Creating the socket using the socket() function
    if((sk = socket(AF_INET, SOCK_STREAM,0)) < 0 ){
        perror("\n Socket could not connect \n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( port);

    //Connecting to Server
    if(connect(sk, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        printf("\n Could not establish connection \n");
        return -1;
    }

    //While loop to read and send commands
    while(1){
        
        //Emptying buffer to avoid errors
        bzero(buffer,1024);
        printf("\nEnter next command: \n");
        fgets(buffer, 1024, stdin);
        buffer[strcspn(buffer, "\r\n")] = 0;
    
        //Closes connection and terminates client if command entered is quit
        if((strncmp("quit", buffer, 4) == 0)){
            send(sk, buffer, strlen(buffer), 0);
            printf("Disconnecting from Server!\n");
            break;
        }else{
            //Sends entered command to Server
            send(sk, buffer, strlen(buffer), 0);  
        }
        bzero(buffer,1024);

        //Reads stdout and stderr from Server
        server_read = read(sk, buffer, 1024);
        printf("%s", buffer);
    }
    close(sk);
    return 0;
}

