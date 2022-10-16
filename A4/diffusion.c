#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

struct parseArgsStruct {
    int iterations;
    float diffusion_constant;
} setup;

typedef struct parseArgsStruct Struct;

Struct parsingArguments(char *argv[]) {

    Struct s;
    if (strchr(argv[1],'d') == NULL) {
    char int1_string[strlen(argv[1]) - 2];
    for (int i = 2; i < strlen(argv[1]); ++i){
      int1_string[i-2] = argv[1][i];
    }
    int int1 = atoi(int1_string);

    char int2_string[strlen(argv[2]) - 2];
    for (int j = 2; j < strlen(argv[2]); ++j){
      int2_string[j-2] = argv[2][j];
    }
    float int2 = atof(int2_string);
    
    s.iterations = int1;
    s.diffusion_constant = int2;

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
    float int2 = atof(int2_string);
    
    s.iterations = int1;
    s.diffusion_constant = int2;
  }
  return s;
}


int main(int argc, char* argv[]) {   
    FILE *f;
    f = fopen("input.txt","r");

    setup = parsingArguments(argv);

    char c;
    int rowCount = 0;
    for (c = getc(f); c != EOF; c = getc(f)) {
        if (c == '\n') {
            rowCount = rowCount + 1;
        }
    }

    rewind(f);
    char *file_contents = malloc(100);
    int sizes[2];

    int counter = 0;
    while (fscanf(f, "%[^-\n ] ", file_contents) != EOF) {
        sizes[counter] = atoi(file_contents);
        counter++;
        if (counter == 2) {
            break;
        }
    }

    float *blockEntries = (float*)malloc(sizeof(float) * sizes[1] * sizes[0]);
    float **block = (float**)malloc(sizeof(float*) * sizes[1]);
    for (size_t j = 0; j < sizes[1]; j++) {
        block[j] = blockEntries + j * sizes[0];
    }

    int currentCoordinates[2];
    int miniCounter=0;
    while (fscanf(f, "%[^-\n ] ", file_contents) != EOF) {
        if ((counter-1)%3) {
            currentCoordinates[miniCounter] = atoi(file_contents);
            counter++;
            miniCounter++;
        } else {
            block[currentCoordinates[0]][currentCoordinates[1]] = atof(file_contents);
            miniCounter = 0;
            counter++;
        }
    }

    //Will no longer use
    fclose(f);
    free(file_contents);
    
                  //Initialize openCL//
    cl_int error;

    //Select platform
    cl_platform_id platform_id;
    cl_uint nmb_platforms;
    if ( clGetPlatformIDs(1, &platform_id, &nmb_platforms) != CL_SUCCESS ) {
      fprintf(stderr, "cannot get platform\n" );
      return 1;
    }

    //Select device
    cl_device_id device_id;
    cl_uint nmb_devices;
    if ( clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &nmb_devices) != CL_SUCCESS ) {
      fprintf(stderr, "cannot get device\n" );
      return 1;
    }
    //Create context for the device
    cl_context context;
    cl_context_properties properties[] =
    {
      CL_CONTEXT_PLATFORM,
      (cl_context_properties) platform_id,
      0
    };
    context = clCreateContext(properties, 1, &device_id, NULL, NULL, &error);
    if ( error != CL_SUCCESS ) {
      fprintf(stderr, "cannot create context\n");
      return 1;
    }

    //Create a command queue for the context on a specific device
    cl_command_queue command_queue;
    command_queue = clCreateCommandQueueWithProperties(context, device_id, NULL, &error);
    if ( error != CL_SUCCESS ) {
      fprintf(stderr, "cannot create command queue\n");
      return 1;
    }

    //Create program for context
    FILE *f_program;
    f_program = fopen("computeNewTemp.cl","r");
    fseek(f_program, 0L, SEEK_END);
    size_t programSize = ftell(f_program);

    //Read computeNewTemp.cl into a buffer
    char *opencl_program_src = (char*)malloc(programSize + 1);
    opencl_program_src[programSize] = 0;
    rewind(f_program);
    fread(opencl_program_src, sizeof(char), programSize, f_program);
    fclose(f_program);

    cl_program program; //Program object for context
    size_t src_len = strlen(opencl_program_src);
    program = clCreateProgramWithSource(context, (cl_uint)1, (const char**)&opencl_program_src, &src_len, &error);
    if (error != CL_SUCCESS)
    {
      fprintf(stderr, "cannot create program\n");
      return 1;
    }

    //Build the program by compiling and linking
    error = clBuildProgram(program, 1, (const cl_device_id*)&device_id, NULL, NULL, NULL);

    if (error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot build program. log:\n");
    
      size_t log_size = 0;
      clGetProgramBuildInfo( program, device_id, CL_PROGRAM_BUILD_LOG,0, NULL, &log_size); // Get log size.

      char * log = calloc(log_size, sizeof(char));
      if (log == NULL) {
        fprintf(stderr,"could not allocate memory\n");
      return 1;
    }
      clGetProgramBuildInfo( program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL); // Get log info.
      fprintf(stderr, "%s\n", log );
    
      free(log);
      return 1;
    }

    //Create Kernel
    cl_kernel kernel = clCreateKernel(program, (const char*)"computeNewTemp", &error);
    if(error != CL_SUCCESS)
    {
      fprintf(stderr, "cannot create kernel dot_prod_mul\n");
      return 1;
    }

    cl_kernel kernel_reduction = clCreateKernel(program, (const char*)"reduction", &error);
    if ( error != CL_SUCCESS ) {
      fprintf(stderr, "cannot create kernel reduction\n");
      return 1;
    }

    //Free and release cl building parts
    free(blockEntries);
    free(block);

    clReleaseContext(context);
    clReleaseProgram(program);
    clReleaseCommandQueue(command_queue);

}