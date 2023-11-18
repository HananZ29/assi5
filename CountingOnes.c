#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int* array ;
int length =1000;
int count;

void *countOnes(void *arg)
{
    int i;
    count = 0;
    
    for (i = 0; i < length; i++)
    {
        if (array[i] == 1)
        {
            count++;
        }
    }
   
    return NULL;
}
int main()
{
   array = (int *)malloc(length * sizeof(int));
    
    for (int i = 0; i < length; i++)
    {
        array[i] = rand() % 6;
    }


   

    pthread_t newthread;
    
    clock_t  start_time = clock();
    
    pthread_create(&newthread, NULL, countOnes, NULL);
    pthread_join(newthread, NULL);
    
    
    clock_t end_time = clock() - start_time;
   
     double time_taken = (double)((end_time - start_time) / CLOCKS_PER_SEC);
    
    printf("Number of ones: %d\n ", count);
   
    printf("Runtime: %.2f seconds\n", time_taken);
    
    free(array);

    system("pause");
    return 0;
}