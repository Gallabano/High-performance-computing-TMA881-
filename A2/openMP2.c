#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <math.h>

static short colSize = 3;
static size_t count[3465]; //Maximal number of distances

static inline void distancesWithinBlock(size_t currentBlockLoad, int** block)
{   
    extern size_t count[];
    int currentCoordinates[colSize], innerProduct;
    int totalDistance;
    size_t i,j;
    for(i = 0; i < currentBlockLoad - 1; ++i) //currentBlockLoad = number of rows in the block
    {
        currentCoordinates[0] = block[i][0];
        currentCoordinates[1] = block[i][1];
        currentCoordinates[2] = block[i][2];
        for(j = i + 1; j < currentBlockLoad; ++j)
        {
            innerProduct = (currentCoordinates[0] - block[j][0])*(currentCoordinates[0] - block[j][0]) +
            (currentCoordinates[1] - block[j][1]) * (currentCoordinates[1] - block[j][1]) +
            (currentCoordinates[2] - block[j][2]) * (currentCoordinates[2] - block[j][2]);
            totalDistance = (int)(sqrt((double)innerProduct)/10 + 0.5);
            
            count[totalDistance] += 1;
            
        }
    }
}

static inline void distancesBetweenBlocks(size_t currentBlockLoad, int** block,int previousBlockValues[colSize])
{
    extern size_t count[];
    int innerProduct, totalDistance;
    size_t i;
    for(i = 0; i < currentBlockLoad; ++i)
    {
        innerProduct = (previousBlockValues[0] - block[i][0]) * (previousBlockValues[0] - block[i][0]) +
        (previousBlockValues[1] - block[i][1]) * (previousBlockValues[1] - block[i][1]) +
        (previousBlockValues[2] - block[i][2]) * (previousBlockValues[2] - block[i][2]);

        totalDistance = (int)(sqrt((double)innerProduct)/10 + 0.5);
        count[totalDistance] += 1;
    }
}

//Maximal theoretical rows = (5*1024*1024)/(bytes of a certain datatype)
int main()
{
    FILE *file;
    file = fopen("test_data/cell_e4","r");
    fseek(file, 0L, SEEK_END);
    long int res = ftell(file); 
    size_t rowCount = res/24; 
    size_t maximumRowsLoad = 10000; 
    size_t numOfBlocksNeeded; 
    size_t maximumRowsInBlock = rowCount; 

    if(rowCount % maximumRowsLoad) //Rows needed
    {
        numOfBlocksNeeded = rowCount/maximumRowsLoad + 1;
    }
    else
    {
        numOfBlocksNeeded = rowCount/maximumRowsLoad;
    }
    
    size_t numOfRowsInBlock[numOfBlocksNeeded], firstBlock[numOfBlocksNeeded]; //Determines number of rows each block receives
    for(size_t i = 0; i < numOfBlocksNeeded; ++i)
    {
        if(maximumRowsInBlock >= maximumRowsLoad)
        {
            numOfRowsInBlock[i] = maximumRowsLoad;
            maximumRowsInBlock -= maximumRowsLoad;
        }
        else
        {
            numOfRowsInBlock[i] = rowCount;
        }
        firstBlock[i] = i * maximumRowsLoad;
    }
    
    for(size_t i = 0; i < numOfBlocksNeeded; ++i) //Start of calculating all blocks
    {
        size_t currentBlockLoad = numOfRowsInBlock[i]; 
        int *blockEntries = (int*)malloc(sizeof(int) * currentBlockLoad * colSize);
        int **block = (int**)malloc(sizeof(int*) * currentBlockLoad);

        for(size_t j = 0; j < currentBlockLoad; ++j)
        {
            block[j] = blockEntries + j * colSize;
        }

        fseek(file, firstBlock[i] * 24L, SEEK_SET);
        for(size_t ix = 0; ix < currentBlockLoad; ++ix)
        {
            char stringLine[25]; 
            fgets(stringLine, 25, file);
            for(size_t jx = 0; jx < colSize; ++jx) //Reads a digit and skip the dot instead of mulitplying by 1000
            {
                char str[6];
                str[0] = stringLine[jx * 8];
                str[1] = stringLine[jx * 8 + 1];
                str[2] = stringLine[jx * 8 + 2];
                str[3] = stringLine[jx * 8 + 4];
                str[4] = stringLine[jx * 8 + 5];
                str[5] = stringLine[jx * 8 + 6];
                int val = atoi(str);
                block[ix][jx] = val;
            }
            
        }
        
        //Calculate distances within a block
        distancesWithinBlock(currentBlockLoad, block);

        if(i > 0)
        {
            size_t previousBlock = i;
            for(size_t ix = 0; ix < previousBlock; ++ix)
            {
                size_t previousBlockStart = firstBlock[ix];
                size_t previousBlockRow = numOfRowsInBlock[ix];
                fseek(file, previousBlockStart * 24L, SEEK_SET);
                int previousBlockValues[colSize];
                for(size_t jx = 0; jx < previousBlockRow; ++jx)
                {
                    char stringLine[25];
                    fgets(stringLine, 25, file);
                    for(size_t kx = 0; kx < colSize; ++kx)
                    {
                        char str[6];
                        str[0] = stringLine[kx * 8];
                        str[1] = stringLine[kx * 8 + 1];
                        str[2] = stringLine[kx * 8 + 2];
                        str[3] = stringLine[kx * 8 + 4];
                        str[4] = stringLine[kx * 8 + 5];
                        str[5] = stringLine[kx * 8 + 6];
                        int val = atoi(str);
                        previousBlockValues[kx] = val;
                    }
                    //Here we calculate the distance between block    
                    distancesBetweenBlocks(currentBlockLoad, block, previousBlockValues);
                }
            }
        }

        free(blockEntries);
        free(block);
    } //End of calculating all blocks
    fclose(file);
    
    size_t test = 0;
    for(size_t i = 0; i < 3465; ++i)
    {   
        test += count[i];
        printf("%.2f\t%ld\n", (double)i/100, count[i]);
    }
    printf("Number of distances: %ld\n", test);
    
}