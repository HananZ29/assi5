#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

// counting one's using one thread length 1000
int *array;
int length = 100000;
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
    *(int *)arg = count;
    pthread_exit(arg);
}
int main()
{
    array = (int *)malloc(length * sizeof(int));

    for (int i = 0; i < length; i++)
    {
        array[i] = rand() % 6;
    }

    clock_t t;
    t = clock();
    for (int i = 0; i < 33; i++)
    {
        pthread_t thread[i];
        pthread_create(&thread[i], NULL, countOnes, (void *)array);
        pthread_join(thread[i], NULL);
    }

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;

    printf("Number of ones: %d\n ", count);

    printf("Runtime: %.2f seconds\n", time_taken);

    free(array);

    system("pause");
    return 0;
}