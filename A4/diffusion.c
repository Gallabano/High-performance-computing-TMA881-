#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main(int argc, char* argv[]) 
{   
    FILE *f;
    f = fopen("init_100_100","r");

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

    double *block = (double*)calloc(sizes[0]*sizes[1], sizeof(double));

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

    free(opencl_program_src);

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

    cl_kernel kernel_difference = clCreateKernel(program, (const char*)"difference", &error);
    if ( error != CL_SUCCESS ) {
      fprintf(stderr, "cannot create kernel difference\n");
      return 1;
    }

    //Create variables within a context
    const int sz = sizes[0] * sizes[1];
    
    cl_mem input_buffer_a, input_buffer_b, output_buffer_c;

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
    if (sizes[0] >= 100)
    {
      local_size_i = 100;
      if (sizes[1] >= 10)
      {
        local_size_j = 10;
      }
      else
      {
        local_size_j = sizes[1];
      }
    }
    else
    {
      if (sizes[0] < 10) {
        local_size_i = sizes[0];
      } else {
        local_size_i = 10;
      }
      if (sizes[1] > 100) {
        local_size_j = 100;
      } else if (sizes[1] < 10) {
        local_size_j = sizes[1];
      } else {
        local_size_j = 10;
      }

    }

    const size_t local_work_sz[] = {local_size_i,local_size_j}; //Local work size
    cl_double diffusion_constant = (cl_double)setup.diffusion_constant;
    cl_int width = (cl_int)sizes[0], height = (cl_int)sizes[1];
    
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
      error = clEnqueueNDRangeKernel(command_queue, kernel , (cl_uint)2, NULL, (const size_t*) &global_sz_szt, (const size_t*)&local_work_sz, 0, NULL, NULL);
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

                           //Reduction part for average temperature
    const size_t global_redsz = 1024;
    const size_t local_redsz = 32;
    const size_t nmb_redgps = global_redsz/local_redsz;

    output_buffer_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, nmb_redgps*sizeof(cl_double), NULL, &error);

    //Set arguments for kernel reduction  
    const cl_int sz_clint = (cl_int)sz;
    if(prevTempArg)
    {
      error = clSetKernelArg(kernel_reduction, 0, sizeof(cl_mem), &input_buffer_b);
      if ( error != CL_SUCCESS) 
      {
        fprintf(stderr, "cannot set kernel_reduction argument for temperature (inside if statement) \n");
        return 1;
      }
    }
    else
    {
      error = clSetKernelArg(kernel_reduction, 0, sizeof(cl_mem), &input_buffer_a);
      if ( error != CL_SUCCESS) 
      {
        fprintf(stderr, "cannot set kernel_reduction argument for temperature (inside else statement) \n");
        return 1;
      }
    }

    error = clSetKernelArg(kernel_reduction, 1, local_redsz*sizeof(cl_double), NULL);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot set kernel_reduction argument for scratch \n");
      return 1;
    }

    error = clSetKernelArg(kernel_reduction, 2, sizeof(cl_int), &sz_clint);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot set kernel_reduction argument for global size \n");
      return 1;
    }

    error = clSetKernelArg(kernel_reduction, 3, sizeof(cl_mem), &output_buffer_c);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot set kernel_reduction argument for output_buffer_c \n");
      return 1;
    }
    //Enqueue the reduction kernel
    error = clEnqueueNDRangeKernel(command_queue, kernel_reduction, 1, NULL, (const size_t*)&global_redsz, (const size_t*)&local_redsz, 0, NULL, NULL);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot enqueue first kernel reduction \n");
      return 1;
    }

    double *c_sum = malloc(nmb_redgps*sizeof(double));
    error = clEnqueueReadBuffer(command_queue, output_buffer_c, CL_TRUE, 0, nmb_redgps*sizeof(cl_double), c_sum, 0, NULL, NULL);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot enqueue read of buffer for kernel reduction \n");
      return 1;
    }
    //Synchronize
    error = clFinish(command_queue);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot finish queue for kernel reduction \n");
      return 1;
    }

    double c_sum_total = 0;
    for (size_t ix = 0; ix < nmb_redgps; ++ix)
    {
      c_sum_total += c_sum[ix];
    }

    double averageTemp = c_sum_total/sz;

                  //Average absolute difference of each temperature to the average of all temperatures

    cl_double averageTemp_clint = (cl_double)averageTemp;
    //Set arguments for kernel difference
    int a_Or_b;
    if(prevTempArg)
    {
      error = clSetKernelArg(kernel_difference, 0, sizeof(cl_mem), &input_buffer_b);
      a_Or_b = 0;
      if ( error != CL_SUCCESS) 
      {
        fprintf(stderr, "cannot set kernel_difference argument for temperature (inside if statement) \n");
        return 1;
      }
    }
    else
    {
      error = clSetKernelArg(kernel_difference, 0, sizeof(cl_mem), &input_buffer_a);
      a_Or_b = 1;
      if ( error != CL_SUCCESS) 
      {
        fprintf(stderr, "cannot set kernel_difference argument for temperature (inside else statement) \n");
        return 1;
      }
    }

    error = clSetKernelArg(kernel_difference, 1, sizeof(cl_double), &averageTemp_clint);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot set kernel_difference argument for average temperature\n");
      return 1;
    }
    
    error = clSetKernelArg(kernel_difference, 2, sizeof(cl_int), &width);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot set kernel_difference argument for width\n");
      return 1;
    }

    //Execute the given arguments in the kernel
    error = clEnqueueNDRangeKernel(command_queue, kernel_difference , 2, NULL, (const size_t*) &global_sz_szt, NULL, 0, NULL, NULL);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot queue kernel_difference arguments\n");
      return 1;
    }

                    //Compute the average of the absolute difference between temperature and average
    error = clEnqueueNDRangeKernel(command_queue, kernel_reduction, 1, NULL, (const size_t*)&global_redsz, (const size_t*)&local_redsz, 0, NULL, NULL);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot enqueue kernel reduction \n");
      return 1;
    }

    error = clEnqueueReadBuffer(command_queue, output_buffer_c, CL_TRUE, 0, nmb_redgps*sizeof(cl_double), c_sum, 0, NULL, NULL);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot enqueue read of buffer for kernel reduction \n");
      return 1;
    }

    error = clFinish(command_queue);
    if ( error != CL_SUCCESS) 
    {
      fprintf(stderr, "cannot finish queue\n");
      return 1;
    }

    c_sum_total = 0;
    for (size_t ix = 0; ix < nmb_redgps; ++ix)
    {
      c_sum_total += c_sum[ix];
    }

    double absAverageTemp = c_sum_total/sz;

    printf("%g\n", averageTemp);
    printf("%g\n", absAverageTemp);
    
    
    //Free and release cl building parts
    free(block);
    free(c_sum);

    clReleaseContext(context);
    clReleaseProgram(program);
    clReleaseCommandQueue(command_queue);
    clReleaseMemObject(input_buffer_a);
    clReleaseMemObject(input_buffer_b);
    clReleaseMemObject(output_buffer_c);
    clReleaseKernel(kernel);
    clReleaseKernel(kernel_reduction);
    clReleaseKernel(kernel_difference);

}