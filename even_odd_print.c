/*
  - Print the numbers in order, while one thread only prints the even numbers and the
    other thread only prints the odd numbers.
  - print synchronusly like 0,1,2,3,4,5 and so forth.
  - Compilation: gcc -o app even_odd_print.c -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


unsigned int shared_data = 0;
pthread_mutex_t mutex;

/* Callback function prototypes*/
void *FuncPrintOdd(void *p);
void *FuncPrintEven(void *p);

int main (void)
{
  pthread_t thread_odd;
  pthread_t thread_even;

  if ((pthread_create(&thread_odd, NULL, FuncPrintOdd, NULL)) < 0) {
    perror ("Odd_thread :");
    exit (EXIT_FAILURE);
  }

  if ((pthread_create(&thread_even, NULL, FuncPrintEven, NULL)) < 0) {
    perror ("Even_thread :");
    exit (EXIT_FAILURE);
  }
  pthread_join(thread_odd, NULL);
  pthread_join(thread_even, NULL);
  return 0;
}

void *FuncPrintOdd(void *p) {
  pthread_mutex_lock(&mutex);
  while (1) {
    if (shared_data % 2 != 0) {
      printf ("Function Odd = %d\n", shared_data);
      sleep (1);
      shared_data++;
    } else {
      pthread_mutex_unlock(&mutex);
    }
  }
}

void *FuncPrintEven(void *p) {
  pthread_mutex_lock(&mutex);
  while (1) {
    if (shared_data % 2 == 0) {
      printf ("Function Even = %d\n", shared_data);
      sleep (1);
      shared_data++;
    } else {
      pthread_mutex_unlock(&mutex);
    }
  }
}
