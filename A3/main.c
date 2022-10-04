#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <math.h>

struct parseArgsStruct {
    int numb_threads, numb_rows, poly_degree;
} setup;

typedef struct parseArgsStruct Struct;

Struct parsingArguments(char *argv[]) {

    Struct s;
    if (strchr(argv[1],'t') == NULL) {
    char int1_string[strlen(argv[1]) - 2];
    for (int i = 2; i < strlen(argv[1]); ++i){
      int1_string[i-2] = argv[1][i];
    }
    int int1 = atoi(int1_string);

    char int2_string[strlen(argv[2]) - 2];
    for (int j = 2; j < strlen(argv[2]); ++j){
      int2_string[j-2] = argv[2][j];
    }
    int int2 = atoi(int2_string);

    s.numb_rows = int1;
    s.numb_threads = int2;

  } else {
    char int1_string[strlen(argv[2]) - 2];
    for (int i = 2; i < strlen(argv[2]); ++i){
      int1_string[i-2] = argv[2][i];
    }
    int int1 = atoi(int1_string);

    char int2_string[strlen(argv[1]) - 2];
    for (int i = 2; i < strlen(argv[1]); ++i){
      int2_string[i-2] = argv[1][i];
    }
    int int2 = atoi(int2_string);

    s.numb_rows = int1;
    s.numb_threads = int2;
  }

  s.poly_degree = atoi(argv[3]);
  return s;
}


int main(int argc, char *argv[])
{

    setup = parsingArguments(argv);
    printf("t=%d, l=%d, deg=%d\n",setup.numb_threads,setup.numb_rows,setup.poly_degree);

    double *attractors = (double*)malloc(sizeof(double)*setup.numb_rows);
    double *convergence = (double*)malloc(sizeof(double)*setup.numb_rows);

    free(attractors);
    free(convergence);
}
