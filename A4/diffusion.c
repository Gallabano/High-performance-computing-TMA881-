#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

struct parseArgsStruct {
    int iterations;
    double diffusion_constant;
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
    double int2 = atof(int2_string);
    
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
    double int2 = atof(int2_string);
    
    s.iterations = int1;
    s.diffusion_constant = int2;
  }
  return s;
}

const char *getErrorString(cl_int error)
{
switch(error){
    // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
}


int main(int argc, char* argv[]) 
{   
    FILE *f;
    f = fopen("input.txt","r");

    setup = parsingArguments(argv);

    char c;
    int rowCount = 0;
    for (c = getc(f); c != EOF; c = getc(f)) 
    {
        if (c == '\n') {
            rowCount = rowCount + 1;
        }
    }

    rewind(f);
    char *file_contents = malloc(100);
    int sizes[2];

    int counter = 0;
    while (fscanf(f, "%[^-\n ] ", file_contents) != EOF) 
    {
        sizes[counter] = atoi(file_contents);
        counter++;
        if (counter == 2) {
            break;
        }
    }
    /*
    double *blockEntries = (double*)malloc(sizeof(double) * sizes[0] * sizes[1]);
    double **block = (double**)malloc(sizeof(double*) * sizes[0]);
    for (size_t j = 0; j < sizes[0]; j++) 
    {
        block[j] = blockEntries + j * sizes[1];
    }
    for (size_t i = 0; i < sizes[0]; ++i)
    {
      for (size_t j = 0; j < sizes[1]; ++j)
      {
        block[i][j] = 0;
      }
    }
    */
    //double *block2 = (double*)calloc(sizes[0] * sizes[1], sizeof(double));
    double *block = (double*)calloc(sizes[0]*sizes[1], sizeof(double));
    //for (size_t i = 0; i < sizes[0]; i++ ) block[i] = calloc( sizes[1], sizeof(double));

    int currentCoordinates[2];
    int miniCounter=0;
    while (fscanf(f, "%[^-\n ] ", file_contents) != EOF) 
    {
        if ((counter-1)%3) {
            currentCoordinates[miniCounter] = atoi(file_contents);
            counter++;
            miniCounter++;
        } else {
            block[currentCoordinates[0]*sizes[0] + currentCoordinates[1]] = atof(file_contents);
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
    if ( clGetPlatformIDs(1, &platform_id, &nmb_platforms) != CL_SUCCESS ) 
    {
      fprintf(stderr, "cannot get platform\n" );
      return 1;
    }

    //Select device
    cl_device_id device_id;
    cl_uint nmb_devices;
    if ( clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &nmb_devices) != CL_SUCCESS ) 
    {
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
    if ( error != CL_SUCCESS ) 
    {
      fprintf(stderr, "cannot create context\n");
      return 1;
    }

    //Create a command queue for the context on a specific device
    cl_command_queue command_queue;
    command_queue = clCreateCommandQueueWithProperties(context, device_id, NULL, &error);
    if ( error != CL_SUCCESS ) 
    {
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

    //Create variables within a context
    const int sz = sizes[0] * sizes[1];
    
    cl_mem input_buffer_a, input_buffer_b;

    input_buffer_a = clCreateBuffer(context, CL_MEM_READ_WRITE, sz*sizeof(cl_double), NULL, &error);
    if ( error != CL_SUCCESS ) 
    {
      fprintf(stderr, "cannot create buffer a\n");
      return 1;
    }
    input_buffer_b = clCreateBuffer(context, CL_MEM_READ_WRITE, sz*sizeof(cl_double), NULL, &error);
    if ( error != CL_SUCCESS ) 
    {
      fprintf(stderr, "cannot create buffer b\n");
      return 1;
    }

    //Pass variables from host to buffer
    error = clEnqueueWriteBuffer(command_queue, input_buffer_a, CL_TRUE, 0, sz*sizeof(cl_double),(const void*)block, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
      fprintf(stderr, "cannot enqueue to write to buffer a\n");
      return 1;
    }

    //Initiate global and local variables to compute the temperature
    const size_t global_sz_szt[] = {sizes[0], sizes[1]};
    size_t local_size_i, local_size_j;
    if (sizes[0] >= 102)
    {
      local_size_i = 102;
      if (sizes[1] >= 10)
      {
        local_size_j = 10;
      }
      else
      {
        local_size_j = sizes[1];
        if (sizes[0] >= 1024/sizes[1])
        {
          local_size_i = 1024/sizes[1];
        }
      }
    }
    else
    {
      local_size_i = sizes[0];
      if (sizes[1] >= 1024/sizes[0])
      {
        local_size_j = 1024/sizes[0];
      }
      else
      {
        local_size_j = sizes[1];
      }
    }

    cl_double diffusion_constant = (cl_double)setup.diffusion_constant;
    cl_int width = (cl_int)sizes[0], height = (cl_int)sizes[1];
    
    const size_t local_work_sz[] = {local_size_i,local_size_j}; //Local work size
    cl_uint prevTempArg = (cl_uint)0, nextTempArg = (cl_uint)1, tempArg;
    for (int iter = 0; iter < setup.iterations; ++iter)
    {
      //Setting Kernel arguments
      error = clSetKernelArg(kernel, prevTempArg, sizeof(cl_mem), &input_buffer_a);
      if (error != CL_SUCCESS)
      {
        fprintf(stderr, "cannot set argument for previous temperature\n");
        return 1;
      }

      error = clSetKernelArg(kernel, nextTempArg, sizeof(cl_mem), &input_buffer_b);
      if (error != CL_SUCCESS)
      {
        fprintf(stderr, "cannot set argument for next temperature\n");
        return 1;
      }

      error = clSetKernelArg(kernel, (cl_uint)2, sizeof(cl_double), &diffusion_constant);
      if (error != CL_SUCCESS)
      {
        fprintf(stderr, "cannot set argument for diffusion constant\n");
        return 1;
      }

      error = clSetKernelArg(kernel, (cl_uint)3, sizeof(cl_int), &width);
      if (error != CL_SUCCESS)
      {
        fprintf(stderr, "cannot set argument for width\n");
        return 1;
      }

      error = clSetKernelArg(kernel, (cl_uint)4, sizeof(cl_int), &height);
      if (error != CL_SUCCESS)
      {
        fprintf(stderr, "cannot set argument for height\n");
        return 1;
      }

      //Queue to execute
      error = clEnqueueNDRangeKernel(command_queue, kernel , (cl_uint)2, NULL, (const size_t*) &global_sz_szt, NULL, 0, NULL, NULL);
      if (error != CL_SUCCESS)
      {
        fprintf(stderr, "cannot enqueue kernel\n");
        return 1;
      }

      //Synchronize
      error = clFinish(command_queue);
      if ( error != CL_SUCCESS) 
      {
        fprintf(stderr, "cannot finish queue\n");
        return 1;
      }

      //Change index to iterate through newest value of temperature
      tempArg = nextTempArg;
      nextTempArg = prevTempArg;
      prevTempArg = tempArg;
    }

    //Free and release cl building parts
    free(block);

    clReleaseContext(context);
    clReleaseProgram(program);
    clReleaseCommandQueue(command_queue);
    clReleaseMemObject(input_buffer_a);
    clReleaseMemObject(input_buffer_b);

}