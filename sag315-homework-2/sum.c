#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#define NUM_THREADS 4

typedef struct data{
    int thread_num;
    int start;
    int end;
} data;

int sum(int start, int end, int total);
void* split_sum(void* s);
int main(int argc, char **argv) {
    
    

    // check input args
    if(!(argc == 1) && !(argc ==3)){
        perror("usage: ./sum\nusage: ./sum <start> <end>\n");
        exit(1);
    }
    int start, end, total = 0, i;
    pthread_t tid[NUM_THREADS];
    data thread_data[NUM_THREADS];
    // set summation bounds
    start = (argc == 3) ? atoi(argv[1]): 1;
    end = (argc == 3) ? atoi(argv[2]): 100;
    int split = (int)((end-start)/NUM_THREADS);

    int sum = 0;
    void* retvalue;
    for(i = 0; i < NUM_THREADS; i++){
        thread_data[i].thread_num = i;
        thread_data[i].start = i == 0 ? start + split* i : thread_data[i-1].end +1;
        thread_data[i].end = i != NUM_THREADS -1 ? start + split * (i+1) : end;
        printf("i = %d: start =  %d end = %d\n", i , thread_data[i].start, thread_data[i].end);
        if((pthread_create(&tid[i],NULL,&split_sum, &thread_data[i])) != 0){
            fprintf(stderr,"Error Creating Thread");
            exit(-1);
        }
    }
    for(i = 0; i < NUM_THREADS; i++){
        pthread_join(tid[i],&retvalue);
        printf("i = %d: %d\n",i,(int)retvalue);
        sum +=(int) retvalue;
    }

    printf("Summation %d - %d = %d\n", start,end,sum);
    pthread_exit(NULL);
    
    return 0;
}
void* split_sum(void* s){
    data sum_data = *((data*)s);
    printf("i = %d: start =  %d end = %d\n", sum_data.thread_num , sum_data.start, sum_data.end);
    int summation = sum(sum_data.start,sum_data.end,0);
    pthread_exit((void*)summation);
}
int sum(int start, int end, int total) {
    return start == end ? total + start : sum(start + 1, end, total + start);
}