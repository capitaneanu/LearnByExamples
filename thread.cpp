#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2
 int thC = 2 ;
/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
  int tid;
  double stuff;
} thread_data_t;
 
/* thread function */
void *thr_func(void *arg) {
      thread_data_t *data = (thread_data_t *)arg;
      int a=0;
      int b=0;
    while(1){
    printf("hello from first thread, thread id: %d\n", b);
    usleep(1);
        a=thC;
        b=a*2;
        if( b > 20)
          break ;
    } 
    pthread_exit(NULL);
}
void *thr_sec(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg  ;
    while(1){
        printf("hello from second thread thread id: %d\n", thC);
        usleep(1);
        thC++ ;
        if(thC == 10)
          thC = 0;
    }
    pthread_exit(NULL);   
}
 
int main(int argc, char **argv) {
  pthread_t thr[NUM_THREADS];
  int i, rc;
  /* create a thread_data_t argument array */
  thread_data_t thr_data[NUM_THREADS];
 
  /* create threads */
  for (i = 0; i < 1; ++i) {
    thr_data[i].tid = i;
    if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) 
    {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  for (i = 1; i < 2; ++i) {
    thr_data[i].tid = i;
    if ((rc = pthread_create(&thr[i], NULL, thr_sec, &thr_data[i]))) 
    {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  }
    printf("Main function still exist\n");
  }
  /* block until all threads complete */
  for (i = 0; i < 1; i++) {
    printf("Main loop call t1 \n");
    pthread_join(thr[i], NULL);
  }
  // After above pthread_join() function main thread will stop and it'll wait for joined thread to finish \
    after thread finished, main function will continue executing from next line of code after pthread_join() function.
    // this code below will not be executed till above pthread_join function ends.
    for (i = 1; i < 2; i++) {
        printf("Main loop call t2 \n");
        pthread_join(thr[i], NULL);
    }
    printf("Main function gonna exit\n");
  return EXIT_SUCCESS;
}