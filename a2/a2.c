#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
/*
pthread_mutex_t mutex_t3;
void* thread_func(void* arg)
{
    int thread_no = *(int*)arg;
    info(BEGIN, 3, thread_no);
    
    if (thread_no == 1)
    {
        pthread_mutex_lock(&mutex_t3);
        info(END, 3, thread_no);
        pthread_mutex_unlock(&mutex_t3);
    }
    else if (thread_no == 3)
    {
        pthread_mutex_lock(&mutex_t3);
        pthread_mutex_unlock(&mutex_t3);
        info(END, 3, thread_no);
    }
    else
    {
        pthread_mutex_lock(&mutex_t3);
        pthread_mutex_unlock(&mutex_t3);
        info(END, 3, thread_no);
    }
    
    return NULL;
}
*/


typedef struct {
    int id_procc;
    int id_thread;
    sem_t *sync;
} TH_STRUCT;


pthread_mutex_t mutex_t3;
sem_t sem_t3;

void* thread_func(void* arg)
{
    int thread_no = *(int*)arg;
    info(BEGIN, 3, thread_no);
    
    
    
    if (thread_no == 1)
    {
        pthread_mutex_lock(&mutex_t3);
        info(END, 3, thread_no);
        pthread_mutex_unlock(&mutex_t3);
    }
    else if (thread_no == 3)
    {
        sem_wait(&sem_t3);
        pthread_mutex_lock(&mutex_t3);
        pthread_mutex_unlock(&mutex_t3);
        info(END, 3, thread_no);
    }
    else
    {
        sem_wait(&sem_t3);
        pthread_mutex_lock(&mutex_t3);
        pthread_mutex_unlock(&mutex_t3);
        info(END, 3, thread_no);
    }
    
    sem_post(&sem_t3); // signal that the thread has finished
    return NULL;
}


void* thread_func8(void* arg)
{

    TH_STRUCT *s = (TH_STRUCT*)arg;
    sem_wait(s->sync);
    info(BEGIN, 8, s->id_thread);
    info(END, 8, s->id_thread);
    sem_post(s->sync);
    return NULL;
}


void* thread_func7(void* arg)
{

    TH_STRUCT *s = (TH_STRUCT*)arg;
    info(BEGIN, 7, s->id_thread);
            info(END, 7, s->id_thread);
    return NULL;
}

void process_hierarchy()
{
    pid_t p2=-1, p3=-1, p4=-1,p5=-1, p6=-1, p7=-1, p8=-1;
   pthread_t t3[5];
    int thread_args[5] = {1, 2, 3, 4, 5};
    sem_t *s1,*s2,*s3,*s4,*s5,*s6,*s7;
    
    s1=sem_open("/sem1",O_CREAT,0644,0);
    p2 = fork();
    
    if(p2==-1){
        perror("Could not create process p2");
        exit(1);
    }
    
    if(p2 == 0)
    {   
    	//begin info 
    	info(BEGIN, 2, 0);
    	s7=sem_open("/sem7",O_CREAT,0644,0);
        p6 = fork();//child process p6
        if(p6 == 0)
        {
            info(BEGIN, 6, 0);
            info(END, 6, 0);
            sem_post(s7);
            exit(0);
        }
        sem_wait(s7);
        sem_unlink("/sem7");
        //end info
        info(END, 2, 0);
        sem_post(s1);
        
        exit(0);
    }
   
  
    s2=sem_open("/sem2",O_CREAT,0644,0);
    p3 = fork();
    if(p3==-1){
        perror("Could not create process p3");
        exit(1);
    }
    if(p3 == 0) 
    {
        info(BEGIN, 3, 0);
        
         
        pthread_mutex_init(&mutex_t3, NULL);
        pthread_create(&t3[0], NULL, thread_func, &thread_args[2]); // T3.3
        pthread_create(&t3[2], NULL, thread_func, &thread_args[0]); // T3.1
        
        pthread_join(t3[2], NULL);
        pthread_join(t3[0], NULL);

        pthread_create(&t3[1], NULL, thread_func, &thread_args[1]);
        pthread_create(&t3[3], NULL, thread_func, &thread_args[3]);
        pthread_create(&t3[4], NULL, thread_func, &thread_args[4]);
        
        pthread_join(t3[1], NULL);
        pthread_join(t3[2], NULL);
        pthread_join(t3[3], NULL);
        pthread_join(t3[4], NULL);
        
        pthread_mutex_destroy(&mutex_t3);
        
        info(END, 3, 0);
        sem_post(s2);
        exit(0);
    }
   
    s3=sem_open("/sem3",O_CREAT,0644,0);
    p4 = fork();
     if(p4==-1){
        perror("Could not create process p4");
        exit(1);
    }
    if(p4 == 0)
    { 
        info(BEGIN, 4, 0);
        info(END, 4, 0);
        sem_post(s3);
        exit(0);
    }
    
    s4=sem_open("/sem4",O_CREAT,0644,0);
    p5 = fork();
     if(p5==-1){
        perror("Could not create process p5");
        exit(1);
    } 
    if(p5 == 0) 
    {
        info(BEGIN, 5, 0);
        s5=sem_open("/sem5",O_CREAT,0644,0);
            
        p8 = fork();
        if(p8==-1){
             perror("Could not create process p8");
             exit(1);
    	} 
        if(p8 == 0)
        {
            info(BEGIN, 8, 0);
            pthread_t tid[47];
    	    TH_STRUCT params[47];
    	    
    	    sem_t sem;
    	    sem_init(&sem,0,6);
    	    
    	     for(int i=0; i<47; i++){
        	params[i].id_procc = 8;
        	params[i].id_thread = i+1;
        	params[i].sync=&sem;
        	pthread_create(&tid[i], NULL, thread_func8, &params[i]);
       	     }

  
       	     for(int i=0; i<47; i++){
                   pthread_join(tid[i], NULL);
        
             } 
    
            info(END, 8, 0);
            sem_post(s5);
            exit(0);
        }
        
        sem_wait(s5);
        sem_unlink("/sem5");
        info(END, 5, 0);
        sem_post(s4);
        exit(0);
    }
   
    s6=sem_open("/sem6",O_CREAT,0644,0);
    p7 = fork();
    if(p7==-1){
        perror("Could not create process p7");
        exit(1);
    }
    if(p7 == 0)
    { 
        info(BEGIN, 7, 0);
        pthread_t tid[4];
    	    TH_STRUCT params[4];
    	    
    	     for(int i=0; i<4; i++){
        	params[i].id_procc = 7;
        	params[i].id_thread = i+1;
        	pthread_create(&tid[i], NULL, thread_func7, &params[i]);
       	     }

  
       for(int i=0; i<4; i++){
            pthread_join(tid[i], NULL);
        
        }
        info(END, 7, 0);
        sem_post(s6);
        exit(0);
    }
    sem_wait(s1);
    sem_wait(s2);
    sem_wait(s3);
    sem_wait(s4);
    sem_wait(s6);
    sem_unlink("/sem1");
    sem_unlink("/sem2");
    sem_unlink("/sem3");
    sem_unlink("/sem4");
    sem_unlink("/sem6");
}

int main()
{
    init();
    info(BEGIN, 1, 0);
    process_hierarchy();
    info(END, 1, 0);
    return 0;
}
