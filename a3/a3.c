#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define REQ_PIPE_NAME "REQ_PIPE_45790"
#define RESP_PIPE_NAME "RESP_PIPE_45790"

/*
#define MAX_STRING_SIZE 250

typedef struct {
    char string_param[MAX_STRING_SIZE];
} Request;

typedef struct {
    unsigned int number_param;
    char string_param[MAX_STRING_SIZE];
} Response;

void sendMessage(int pipe, const void* message, size_t size) {
    write(pipe, message, size);
}

void receiveMessage(int pipe, void* buffer, size_t size) {
    read(pipe, buffer, size);
}
*/

int main() {
    int respPipe=-1, reqPipe=-1;
    //Request request;
   // Response response;
   // strcpy(request.string_param,"PING");
   // strcpy(response.string_param,"PING PONG 45790");

   
    //create respPipe
    if (mkfifo(RESP_PIPE_NAME, 0600) !=0) {
        printf("ERROR\ncannot create the response pipe\n");
        return 1;
    }

    //open the reqPipe
    reqPipe = open(REQ_PIPE_NAME, O_RDONLY);
    if (reqPipe == -1) {
        printf("ERROR\ncannot open the request pipe\n");
        //unlink(RESP_PIPE_NAME); 
        return 1;
    }

    //open respPipe 
    respPipe = open(RESP_PIPE_NAME, O_WRONLY);
    if (respPipe == -1) {
        close(reqPipe);
        //unlink(RESP_PIPE_NAME);  
        return 1;
    }

    //write in pipe-ul resp
    write(respPipe,"5HELLO",6);
    printf("SUCCESS\n");
    
 
    /*
    while (1) {
        // Receive request
        receiveMessage(reqPipe, &request, sizeof(Request));
         if(strcmp(&request,"PING") == 0)
        {
            // Write back response
            sendMessage(respPipe, &response, sizeof(Response));
            break;
        }
    }*/


    return 0;
}
