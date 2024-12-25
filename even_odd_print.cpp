/*
  - This program demonstrates the use of threads to print numbers in order synchronously, 
    where one thread prints only even numbers and the other prints only odd numbers.
  - Numbers are printed in the sequence 0, 1, 2, 3, 4, ...
  - Compilation: g++ -o app even_odd_print.cpp -lpthread
*/

#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

// Macros for readability
#define SLEEP_DURATION 1 // Duration for simulated work
#define ODD 1            // Check for odd numbers
#define EVEN 0           // Check for even numbers

class EvenOddPrinter {
private:
    unsigned int current_number; // Shared variable to store the current number to be printed.
    pthread_mutex_t number_mutex; // Mutex to ensure synchronization between threads.

public:
    EvenOddPrinter() : current_number(0) {
        pthread_mutex_init(&number_mutex, NULL); // Initialize the mutex.
    }

    ~EvenOddPrinter() {
        pthread_mutex_destroy(&number_mutex); // Destroy the mutex.
    }

    // Thread function to print odd numbers.
    static void *printOddNumbers(void *arg) {
        EvenOddPrinter *printer = static_cast<EvenOddPrinter *>(arg);
        while (true) {
            pthread_mutex_lock(&printer->number_mutex); // Acquire the lock.
            if (printer->current_number % 2 == ODD) { // Check if the number is odd.
                cout << "Odd Thread: " << printer->current_number << endl;
                sleep(SLEEP_DURATION); // Simulate work.
                printer->current_number++; // Increment the shared variable.
            }
            pthread_mutex_unlock(&printer->number_mutex); // Release the lock.
        }
    }

    // Thread function to print even numbers.
    static void *printEvenNumbers(void *arg) {
        EvenOddPrinter *printer = static_cast<EvenOddPrinter *>(arg);
        while (true) {
            pthread_mutex_lock(&printer->number_mutex); // Acquire the lock.
            if (printer->current_number % 2 == EVEN) { // Check if the number is even.
                cout << "Even Thread: " << printer->current_number << endl;
                sleep(SLEEP_DURATION); // Simulate work.
                printer->current_number++; // Increment the shared variable.
            }
            pthread_mutex_unlock(&printer->number_mutex); // Release the lock.
        }
    }

    void run() {
        pthread_t odd_thread;  // Thread for printing odd numbers.
        pthread_t even_thread; // Thread for printing even numbers.

        // Create the thread for printing odd numbers.
        if (pthread_create(&odd_thread, NULL, printOddNumbers, this) < 0) {
            perror("Failed to create thread for odd numbers");
            exit(EXIT_FAILURE);
        }

        // Create the thread for printing even numbers.
        if (pthread_create(&even_thread, NULL, printEvenNumbers, this) < 0) {
            perror("Failed to create thread for even numbers");
            exit(EXIT_FAILURE);
        }

        // Wait for both threads to complete execution.
        pthread_join(odd_thread, NULL);
        pthread_join(even_thread, NULL);
    }
};

int main() {
    EvenOddPrinter printer;
    printer.run(); // Start the threads to print even and odd numbers.
    return 0;
}
