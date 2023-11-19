
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define L1_CACHE_SIZE 320 // 320 KB

struct ThreadData
{
    int privateCount;
    char padding[L1_CACHE_SIZE - sizeof(int)]; // Padding to fill the cache line
};

long *array;
long length = 100000000;
int numberOfThreads = 32;



struct ThreadData *privateCounts; // Array of private counts with padding

void *countOnes(void *thread_num)
{
    long THREAD_NUM = *((long *)thread_num);
    int local_count = 0;
    long start = THREAD_NUM * (length / numberOfThreads); // starting point of the array
    int end = start + length / numberOfThreads;           // end point of the array

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

    privateCounts[THREAD_NUM].privateCount = local_count;
    pthread_exit(thread_num);
}

int countOnesSeq()
{
    int i;
    int countSequential = 0;

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
    array = (long *)malloc(length * sizeof(long));
    privateCounts = (struct ThreadData *)malloc(numberOfThreads * sizeof(struct ThreadData));

    int correctCounter = 0; // to count the number of times the program returns the correct output

    pthread_t thread[numberOfThreads];
    double time_taken;
    for (int run = 0; run < 100; run++)
    {
        clock_t t;
        t = clock();
        for (int i = 0; i < length; i++)
        {
            array[i] = rand() % 5 + 1;
        }

        int countSequential = countOnesSeq();

        for (int i = 0; i < numberOfThreads; i++)
        {
            long *iptr = malloc(sizeof(long)); // Allocate memory for thread-specific data
            *iptr = i;
            pthread_create(&thread[i], NULL, countOnes, (void *)iptr);
        }

        for (int i = 0; i < numberOfThreads; i++)
        {
            pthread_join(thread[i], NULL);
        }

        int count = 0;
        for (int i = 0; i < numberOfThreads; i++)
        {
            count += privateCounts[i].privateCount;
        }

        if (count == countSequential)
        {
            correctCounter++;
        }
        t = clock() - t;
        time_taken = ((double)t) / CLOCKS_PER_SEC;
    }

    printf("Number of correct executions: %d\n", correctCounter);
    printf("Runtime: %.2f seconds\n", time_taken);
    free(array);
    free(privateCounts);

    system("pause");
    return 0;
}
