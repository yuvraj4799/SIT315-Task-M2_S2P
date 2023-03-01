// Header files providing functionality to the program
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <string>
using namespace std;

// Global variables that will be used in the program
#define NUM_THREADS 8
#define MAX 1000000
pthread_mutex_t mutx;

int data[MAX];
long global_sum = 0;

void *sum(void *threadid)
{
    // To get Thread ID
    long tid;
    long sum = 0;
    tid = (long)threadid;
    // printf("Bye World! It's me, thread #%ld!\n", tid);
    int range = MAX / NUM_THREADS;
    int start = tid * range;
    int end = start + range;

    // Indices of the Data range
    for (int i = start; i < end; i++)
    {
        sum += data[i];
    }

    // enforce sequential update
    pthread_mutex_lock(&mutx);
    global_sum += sum;

    pthread_mutex_unlock(&mutx);

    // enforce sequential update

    pthread_exit(NULL);
}

// The main function of the Program
int main(int argc, char *argv[])
{
    // Determing the number of available hardware cores
    int cores = std::thread::hardware_concurrency();
    pthread_mutex_init(&mutx, NULL);

    cout << " the number of cores on this machine = " << cores << endl;
    // srand(time(NULL));
    for (int i = 0; i < MAX; i++)
    {
        // Storing the aray with random arrays
        data[i] = rand() % 20;
    }

    pthread_t threads[NUM_THREADS];
    long tid = 0;

    // Loops over the array, creating a new thread tfor each object
    for (tid = 0; tid < NUM_THREADS; tid++)
    {
        pthread_create(&threads[tid], NULL, sum, (void *)tid);
    }

    // Each thread is given the ID
    for (tid = 0; tid < NUM_THREADS; tid++)
    {
        pthread_join(threads[tid], NULL);
    }

    cout << "the final sum =" << global_sum << endl;
}