#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int *array;
int length = 100000000;
int numberOfThreads = 32;
int count;
int countSequential;
pthread_mutex_t mutex;  // Mutex to guard the critical section

void *countOnes(void *thread_num)
{
    int THREAD_NUM = *((int *)thread_num);
    int local_count = 0;
    int start = THREAD_NUM * (length / numberOfThreads); // starting point of the array
    int end = start + length / numberOfThreads;          // end point of the array

    if (length - end < (length / numberOfThreads))
    {
        end = length;
    }

    for (start; start < end; start++)
    {
        if (array[start] == 1)
        {
            local_count++;
        }
    }

    // Lock the critical section before updating the shared count variable
    pthread_mutex_lock(&mutex);
    count += local_count;
    // Unlock the critical section after updating the shared count variable
    pthread_mutex_unlock(&mutex);

    pthread_exit(thread_num);
}

int countOnesSeq()
{
    int i;
    countSequential = 0;

    for (i = 0; i < length; i++)
    {
        if (array[i] == 1)
        {
            countSequential++;
        }
    }

    return countSequential;
}

int main()
{
    array = (int *)malloc(length * sizeof(int));

    int correctCounter = 0; // to count the number of times the program returns the correct output

    int s;
    pthread_t thread[numberOfThreads];
    pthread_mutex_init(&mutex, NULL); // Initialize the mutex

     double time_taken ;
     for (int i = 0; i < length; i++)
        {
            array[i] = rand() % 5 + 1;
        }

        countSequential = countOnesSeq();
    for (int run = 0; run < 100; run++)
    {
       

        int *numPtr = &numberOfThreads;
        clock_t t;
        t = clock();

        for (int i = 0; i < numberOfThreads; i++)
        {
            int *iptr = malloc(sizeof(int)); // Allocate memory for thread-specific data
            *iptr = i;
            pthread_create(&thread[i], NULL, countOnes, (void *)iptr);
        }

        for (int i = 0; i < numberOfThreads; i++)
        {
            pthread_join(thread[i], NULL);
        }

        t = clock() - t;
        time_taken = ((double)t) / CLOCKS_PER_SEC;
        if (count == countSequential)
        {
            correctCounter++;
        }
    }

    printf("Number of correct executions: %d\n ", correctCounter);
    printf("Runtime: %.2f seconds\n", time_taken);
    free(array);
    pthread_mutex_destroy(&mutex); 
    system("pause");
    return 0;
}
