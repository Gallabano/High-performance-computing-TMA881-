#include <stdio.h>
#include <time.h>
#include "different_file_mul.h"

int main()
{
    #define SIZE 30000
    double as_re[SIZE]; 
    double bs_re[SIZE];
    double cs_re[SIZE];
    double as_im[SIZE]; 
    double bs_im[SIZE];
    double cs_im[SIZE];

    size_t iter = 200000;

    clock_t begin = clock();
    for (int i = 0; i < iter; ++i)
    {
        for (size_t j = 0; j < SIZE; ++j)
        {
            bs_re[j] = 1; bs_im[j] = 2;
            cs_re[j] = 1; cs_im[j] = 2;

            different_file_mul(&as_re[j], &as_im[j], &bs_re[j], &bs_im[j], &cs_re[j], &cs_im[j]);
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;

    printf("Elapsed time of %d iterations: %f\n", iter, time_spent);
    printf("First element of as with real and imaginary parts respectively: %f, %f\n", as_re[0], as_im[0]);

    return 0;
}