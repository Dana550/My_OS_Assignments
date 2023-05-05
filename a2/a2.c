#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"

void process_hierarchy()
{
    pid_t p2=-1, p3=-1, p4=-1,p5=-1, p6=-1, p7=-1, p8=-1;

    p2 = fork();
    
    if(p2==-1){
        perror("Could not create process p2");
        exit(1);
    }
    
    if(p2 == 0)
    {   
    	//begin info 
    	info(BEGIN, 2, 0);
    	
        p6 = fork();//child process p6
        if(p6 == 0)
        {
            info(BEGIN, 6, 0);
            info(END, 6, 0);
            
            exit(0);
        }
        wait(NULL); //P6
        //end info
        info(END, 2, 0);
        
        exit(0);
    }
    //wait after every process:
    wait(NULL); //P2
    
    
    p3 = fork();
    if(p3==-1){
        perror("Could not create process p3");
        exit(1);
    }
    if(p3 == 0) 
    {
        info(BEGIN, 3, 0);
        info(END, 3, 0);
        
        exit(0);
    }
    wait(NULL); //P3
    
    p4 = fork();
     if(p4==-1){
        perror("Could not create process p4");
        exit(1);
    }
    if(p4 == 0)
    { 
        info(BEGIN, 4, 0);
        info(END, 4, 0);
        
        exit(0);
    }
    wait(NULL); //P4
    
    p5 = fork();
     if(p5==-1){
        perror("Could not create process p5");
        exit(1);
    } 
    if(p5 == 0) 
    {
        info(BEGIN, 5, 0);
        
        p8 = fork();
        if(p8==-1){
             perror("Could not create process p8");
             exit(1);
    	} 
        if(p8 == 0)
        {
            info(BEGIN, 8, 0);
            info(END, 8, 0);
            exit(0);
        }
        wait(NULL); //P8
        
        info(END, 5, 0);
        exit(0);
    }
    wait(NULL); //P5
    
    p7 = fork();
    if(p7==-1){
        perror("Could not create process p7");
        exit(1);
    }
    if(p7 == 0)
    { 
        info(BEGIN, 7, 0);
        info(END, 7, 0);
        exit(0);
    }
    wait(NULL); //P7
   
}

int main()
{
    init();
    info(BEGIN, 1, 0);
    process_hierarchy();
    info(END, 1, 0);
    return 0;
}
