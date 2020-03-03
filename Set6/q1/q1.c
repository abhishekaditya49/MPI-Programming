#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <CL/cl.h>


#define DATA_SIZE (1024)


int main () {

	int err;

	size_t global_work_size;
	size_t local_work_size;

	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_context context;
	cl_command_queue command_queue;
	cl_program program;
	cl_kernel kernel;

	cl_mem mem_input;
	cl_mem mem_output;



	// Ain't no body got time to take input :p
	char data[20];
	printf("Enter a string\n");
	gets(data);
	unsigned int count = strlen(data);
	int n;
	printf("Enter n\n");
	scanf("%d",&n);
	char *results = (char *)malloc(count * n * sizeof(char));


	// 1. Load the kernel code for the glory of the Sontaran Empire

	FILE *kernel_code_file = fopen("q1kernel.cl", "r");
	if (kernel_code_file == NULL) {
		printf("Kernel loading failed.");
		exit(1);
	}

	char *source_str = (char *)malloc(DATA_SIZE);
	size_t source_size = fread(source_str, 1, DATA_SIZE, kernel_code_file);
	source_str[source_size] = '\0'; // VERY IMPORTANT, cause random C stuff.

	fclose(kernel_code_file);



	// 2. Get platform and device information (Connect to a compute device)

	err = clGetPlatformIDs(1, &platform_id, NULL);
	err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);





	// 3. Create an OpenCL context

	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

	if (!context) {
		printf("Error: Failed to create a compute context!\n");
		return EXIT_FAILURE;
	}



	// 4. Create a command queue

	command_queue = clCreateCommandQueue(context, device_id, 0, &err);

	if (!command_queue) {
		printf("Error: Failed to create a command commands!\n");
		return EXIT_FAILURE;
	}



	// 5. Create memory buffers on the device for the kernel args

	mem_input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(char) * count, NULL, NULL);
	mem_output = clCreateBuffer(context,  CL_MEM_WRITE_ONLY,  sizeof(char) * count * n, NULL, NULL);




	// 6. Write the memory contents to the device memory

	err = clEnqueueWriteBuffer(command_queue, mem_input, CL_TRUE, 0, sizeof(char) * count, data, 0, NULL, NULL);




	// 7. Create a program from kernel source

	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, NULL, &err);




	// 8. Build the kernel program executable

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);





	// 9. Create the OpenCL kernel object in the program we wish to run

	kernel = clCreateKernel(program, "multiply", &err);

	if (!kernel || err != CL_SUCCESS) {
		printf("Error: Failed to create compute kernel!\n");
		exit(1);
	}



	// 10. Set the arguments to our compute kernel

	err  = 0;
	err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem_input);
	err |= clSetKernelArg(kernel, 1, sizeof(int), &n);
	err |= clSetKernelArg(kernel, 2, sizeof(int), &count);
	err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &mem_output);

	if (err != CL_SUCCESS) {
		printf("Error: Failed to set kernel arguments! %d\n", err);
		exit(1);
	}



	// 11. Execute the kernel on device

	global_work_size = count;
	local_work_size = 1;

	printf("Global work size = %d, Local work size = %d\n", global_work_size, local_work_size);

	err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);

	if (err) {
		printf("Error: Failed to execute kernel!\n");
		return EXIT_FAILURE;
	}

 	clFinish(command_queue); // Wait for the command commands to get serviced before reading back results



	// 12. Read the memory buffer from the device memory and copy to local memory

	err = clEnqueueReadBuffer(command_queue, mem_output, CL_TRUE, 0, sizeof(char) * count * n, results, 0, NULL, NULL );

	if (err != CL_SUCCESS) {
		printf("Error: Failed to read output array! %d\n", err);
		exit(1);
	}

	printf("%s\t|\t%s\n", data, results);



	// 13. Clean shit up

	clReleaseMemObject(mem_input);
	clReleaseMemObject(mem_output);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	return 0;
}
