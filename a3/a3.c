#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define RESP_PIPE_NAME "RESP_PIPE_45790"
#define REQ_PIPE_NAME "REQ_PIPE_45790"


int main() {
    int respPipe=-1, reqPipe=-1;
  
    //create respPipe
    if (mkfifo(RESP_PIPE_NAME, 0600) !=0) {
        printf("ERROR\ncannot create the response pipe");
        return 2;
    }

    reqPipe = open(REQ_PIPE_NAME, O_RDONLY);
    if (reqPipe == -1) 
    {
        printf("ERROR\ncannot open the request pipe");
        return 1;
    }

    respPipe = open(RESP_PIPE_NAME, O_WRONLY);
    if (respPipe == -1) 
    {
        return 1;
    }

    //write in pipe-ul resp
    char string[7]="HELLO";
    unsigned int length=strlen(string);
    write(respPipe, &length, 1);
    write(respPipe,string,length);    
    printf("SUCCESS\n");

    unlink(REQ_PIPE_NAME);
    unlink(RESP_PIPE_NAME);
 
    return 0;
}
