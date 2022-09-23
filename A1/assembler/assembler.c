#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]){
    FILE *f;
    unsigned long long a = 0;
    int ix = 0;
    clock_t begin = clock();
    for (ix = 0; ix <= 1000000000; ++ix){
        a += ix;
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;

    f = fopen("naiveBenchmarking.txt", "a");
    fprintf(f, "\nValue of sum: %llu. It took %f s using optimization: %s", a ,time_spent, argv[1]);
    fclose(f);
}