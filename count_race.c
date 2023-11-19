
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

long *array;
long length = 500000000;
int numberOfThreads =2;
int count;
int countSequential;
void *countOnes(void * thread_num)
{
    long THREAD_NUM = *((long*)thread_num);
    count = 0;
    long start = *((long*)thread_num);                // starting point of the array
    int end = start + length / *((int *)thread_num); // end point of the array
    if(length-end<(length / *((int *)thread_num))){
        end = length;
    }
    for (start = 0; start < length; start++)
    {
        if (array[start] == 1)
        {
            count++;
        }
    }
     *(int *)thread_num = count;
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

    array = (long *)malloc(length * sizeof(long));

    int correctCounter = 0; // to count the number of times the program returns the correct output

    int s;
    pthread_t thread[numberOfThreads];
    for (int run = 0; run < 100; run++)
    {

        
    for (int i = 0; i < length; i++)
    {
        array[i] = rand() % 5 +1;
    }
    
     countSequential = countOnesSeq();
   
       
        int *numPtr = &numberOfThreads;
        clock_t t;
        t = clock();

        for (int i = 0; i < numberOfThreads; i++)
        {
            int*iptr = &i;
            pthread_create(&thread[i], NULL, countOnes, ((void *)iptr));
          
        }
         for (int i = 0; i < numberOfThreads; i++)
        {
            
            

            pthread_join(thread[i], NULL);
          
        }

        t = clock() - t;
        double time_taken = ((double)t) / CLOCKS_PER_SEC;
        if (count == countSequential)
        {
            correctCounter++;
        }
    }

    printf("Number of correct executions: %d\n ", correctCounter);

    free(array);

    system("pause");
    return 0;
}