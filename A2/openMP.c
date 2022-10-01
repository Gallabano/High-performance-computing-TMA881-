#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <math.h>
static inline float dist(int x1, int y1, int z1, int x2, int y2, int z2)
{   
    int xdiff = x1 - x2;
    int ydiff = y1 - y2;
    int zdiff = z1 - z2;
    float d = sqrtf(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff);
    return d;
}

static inline void swap(float* xp, float* yp)
{
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// Function to perform Selection Sort
static inline void selectionSort(float arr[],long int n)
{
    long int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {
 
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
 
        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

static inline void occurences(float *distances,long int *freq,long int numDistances)
{   
    long int i,j; //Number of distances may be a lot
    int count;
    for(i = 0; i <numDistances; i++)
    {
        count=1;
        if(distances[i]!=-1)
		{
		    for(j=i+1; j<numDistances; j++)
     
            {
        	   if(distances[i]==distances[j])
        	    {
			       count++;
			       distances[j]=-1;
		       }
	       }
	       freq[i]=count;
		}          
    }
 }
 
int main()
{   
    int colSize = 3;
    int rowCount = 0;
    float val;

    FILE *file;
    file = fopen("test_data/cell_e3","r");

    char str[8];

    fseek(file, 0L, SEEK_END);
    long int res = ftell(file); 
    rowCount = res/24;

    long int numDistances = (rowCount*(rowCount - 1))/2;
    long int count = 0;
    float *distances = (float*)malloc(sizeof(float)*numDistances);
    //long int *freq =(long int*)malloc(sizeof(long int*)*numDistances);

    rewind(file); //Reset the file pointer to the beginning

    int *matrixEntries = (int*)malloc(sizeof(int) * rowCount * colSize); //Contiguous memory block
    int **matrix = (int**)malloc(sizeof(int*) * rowCount); 
    for ( int ix = 0; ix < rowCount; ++ix ){
        matrix[ix] = matrixEntries + ix * colSize;
    }
    
    
    int i,j,dummy;
    i = j = 0;
    while(fscanf(file," %7[^\n]%*c",str) == 1) //String -> float -> int (by multiplying float with 1000)
    {
        val = atof(str)*1000;
        dummy = (int)val;
        matrix[i][j] = dummy;
        //printf("%d\n", matrix[i][j]);
        if(j == 2)
        {
            if(i > 0 && count < numDistances)
            {
                for(long int ix = i - 1; ix > -1; --ix)
                {   
                    distances[count] = dist(matrix[i][0],matrix[i][1],matrix[i][2],matrix[ix][0],matrix[ix][1],matrix[ix][2])/1000;
                    ++count;
                }
            }            
            ++i;
            j = 0;

        }
        else
        {
            ++j;
            
        }

    }
    
    fclose(file);
    free(matrixEntries);
    free(matrix);
    
    selectionSort(distances,numDistances);
    //occurences(distances,freq,numDistances);
    
    for(long int ix = 0; ix < numDistances; ++ix)
    {
        printf("%.2f\n", distances[ix]);
    }
    free(distances);
    //free(freq);
    
    
    return 0;
}

