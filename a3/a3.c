#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/mman.h>

#define RESP_PIPE_NAME "RESP_PIPE_45790"
#define REQ_PIPE_NAME "REQ_PIPE_45790"

int main() {
    int respPipe = -1, reqPipe = -1;

    //create respPipe
    if (mkfifo(RESP_PIPE_NAME, 0600) != 0) {
        printf("ERROR\ncannot create the response pipe");
        return 2;
    }

    reqPipe = open(REQ_PIPE_NAME, O_RDONLY);
    if (reqPipe == -1) {
        printf("ERROR\ncannot open the request pipe");
        return 1;
    }

    respPipe = open(RESP_PIPE_NAME, O_WRONLY);
    if (respPipe == -1) {
        return 1;
    }

    //write in respPipe
    char string[7] = "HELLO";
    unsigned int length = strlen(string);
    write(respPipe, &length, 1);
    write(respPipe, string, length);
    printf("SUCCESS\n");

    char* read_from = NULL;

    int shmFd = -1;
 
    char* shmPr= NULL;

   // unsigned int number = 0;

    while (1) {
        //receive request
        unsigned int a = 0;
        read(reqPipe, &a, 1);
        read_from = (char*)malloc(a * sizeof(char));
        read(reqPipe, read_from, a);
        read_from[a] = '\0';

        if (strcmp(read_from, "EXIT") == 0) {
            close(reqPipe);
            close(respPipe);
            unlink(RESP_PIPE_NAME);
            break;
        }

        if (strcmp(read_from, "PING") == 0) {
            char ping_pong[7] = "PING";
            unsigned int length = strlen(ping_pong);

            write(respPipe, &length, 1);
            write(respPipe, ping_pong, length);
            write(respPipe, &length, 1);
            write(respPipe, "PONG", length);
            unsigned int variant = 45790;
            write(respPipe, &variant, sizeof(variant));
            break;
        }

        if (strcmp(read_from, "CREATE_SHM") == 0) {
            int size = 0;
            read(reqPipe, &size, sizeof(size));

            shmFd = shm_open("/7moqaM", O_CREAT | O_RDWR, 0664);
            if (shmFd < 0) {
                char create[20] = "CREATE_SHM";
                int length1=strlen(create);
                char error[20] = "ERROR";
                int length2=strlen(error);

                write(respPipe,&length1,1);
                write(respPipe,create,10);   

                write(respPipe,&length2,1);
                write(respPipe,error,5);  
            }

            // Adjust the size of the shared memory region
            ftruncate(shmFd, size);
           
            // Map the shared memory region into the virtual address space
            shmPr = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE,MAP_SHARED,shmFd, 0);
            if (shmPr == (void*)-1) {
                char create[20] = "CREATE_SHM";
                int length1=strlen(create);
                char error[20] = "ERROR";
                int length2=strlen(error);

                write(respPipe,&length1,1);
                write(respPipe,create,10);   

                write(respPipe,&length2,1);
                write(respPipe,error,5);
                shm_unlink("/7moqaM");            
            }

            // Close the file descriptor
            close(shmFd);

            char create[20] = "CREATE_SHM";
            int length1=strlen(create);
            char error[20] = "SUCCESS";
            int length2=strlen(error);

            write(respPipe,&length1,1);
            write(respPipe,create,10);   
            write(respPipe,&length2,1);
            write(respPipe,error,7);
        }
        /*
        if (strcmp(read_from, "WRITE_TO_SHM") == 0) {
            unsigned int offset = 0, value = 0;
            read(reqPipe, &offset, sizeof(offset));
            read(reqPipe, &value, sizeof(value));

            if (offset >= 4673015 || offset + sizeof(value) > 4673015) {
                char error[] = "WRITE_TO_SHM ERROR";
                unsigned int errorLength = strlen(error);
                write(respPipe, &errorLength, sizeof(errorLength));
                write(respPipe, error, errorLength);
            } else {
                // Write value to the shared memory at the specified offset
                unsigned int* shmOffsetPtr = (unsigned int*)((char*)shmPr + offset);
                *shmOffsetPtr = value;

                char response[] = "WRITE_TO_SHM SUCCESS";
                unsigned int responseLength = strlen(response);
                write(respPipe, &responseLength, sizeof(responseLength));
                write(respPipe, response, responseLength);
            }
        }*/

        free(read_from); // Deallocate the memory
        break;
    }
    close(reqPipe);
    close(respPipe);

    unlink(REQ_PIPE_NAME);
    unlink(RESP_PIPE_NAME);
    return 0;
}

