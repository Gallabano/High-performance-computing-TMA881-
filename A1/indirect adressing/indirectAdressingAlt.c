#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{   
    size_t size = 1000000;
    int a = 2;
    
    size_t *y = (size_t*)malloc(sizeof(size_t)*size);
    size_t *x = (size_t*)malloc(sizeof(size_t)*size);

    size_t count = 0;
    clock_t begin = clock();
    for ( size_t ix = 0; ix < size; ++ix  ) //Generating p linearly
    {
        x[ix] = (size_t)3;
        y[ix] = (size_t)1;
    }
        
    
    for ( size_t kx = 0; kx < size; ++kx ) 
    {
        y[kx] += a * x[kx];
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;

    printf("Elapsed time: %f s\n", time_spent);
    printf("Random element of sums: %zu\n", y[0]);

    free(y);
    free(x);

    return 0;
}