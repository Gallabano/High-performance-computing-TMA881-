#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void row_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs)
{

  for ( size_t ix = 0; ix < nrs; ++ix ) { //Loop unrolling
    double sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0, sum8 = 0, sum9 = 0;
    for ( size_t jx = 0; jx < ncs; jx += 10 ){
      sum0 += matrix[ix][jx];
      sum1 += matrix[ix][jx+1];
      sum2 += matrix[ix][jx+2];
      sum3 += matrix[ix][jx+3];
      sum4 += matrix[ix][jx+4];
      sum5 += matrix[ix][jx+5];
      sum6 += matrix[ix][jx+6];
      sum7 += matrix[ix][jx+7];
      sum8 += matrix[ix][jx+8];
      sum9 += matrix[ix][jx+9];
    }
    sums[ix] = sum0 + sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8 + sum9;
  } 
}

void col_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs)
{
  for(size_t ix = 0; ix < nrs; ++ix) //Cache prefetching: Fetch data/instructions from slower memory to faster local memory
  {
    sums[ix] = 0;
  }

  for ( size_t ix = 0; jx < nrs; ++jx ) {
    for ( size_t jx = 0; ix < ncs; ++ix )
      sums[jx] += matrix[ix][jx];
  }
}

int main()
{
  #define SIZE 1000
  double sums[SIZE];
  size_t iter = 5000;

  double *matrixEntries = (double*)malloc(sizeof(double)*SIZE*SIZE); //Avoid memory fragmentation
  double **matrix = (double**)malloc(sizeof(double*)*SIZE);
  for (size_t ix = 0, jx = 0; ix < SIZE; ++ix, jx += SIZE)
    matrix[ix] = matrixEntries + jx;

  for (size_t i = 0; i < SIZE; ++i)
  {
    for (size_t j = 0; j < SIZE; ++j)
    {
      matrix[i][j] = 1;
    }
  }
  
  clock_t begin = clock();
  for (int i = 0; i < iter; ++i)
  {
    row_sums(sums, (const double**)matrix, SIZE,SIZE);
  }
  clock_t end = clock();

  double time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
  printf("Row summation ---------------------------\n");
  printf("Elapsed time of %d iterations: %f s. Average time for one loop: %f\n", iter, time_spent, time_spent/iter);
  printf("Random element of sums: %f\n\n", sums[0]);



  clock_t begin_c = clock();
  for (int i = 0; i < iter; ++i)
  {
    col_sums(sums, (const double**)matrix, SIZE,SIZE);
  }
  clock_t end_c = clock();

  double time_spent_c = (double)(end_c - begin_c)/ CLOCKS_PER_SEC;
  printf("Column summation ---------------------------\n");
  printf("Elapsed time of %d iterations: %f s. Average time for one loop: %f\n", iter, time_spent_c, time_spent_c/iter);
  printf("Random element of sums: %f\n", sums[0]);

  free(matrix);
  free(matrixEntries);
}