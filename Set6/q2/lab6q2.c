#include <stdio.h>
#include <CL/cl.h>
#include <stdlib.h>

#define MAX_SIZE 0x100000

int main(){

	int i;
	int LIST_SIZE;

	printf("Enter number of elements :");
	scanf("%d",&LIST_SIZE);

	FILE *fp;
	char *stream;
	size_t size;
	fp = fopen("q2kernel.cl","r");

	stream = (char*) malloc(sizeof(char)*MAX_SIZE);

	size = fread(stream,sizeof(char),MAX_SIZE,fp);

	fclose(fp);

	int *A = (int*) malloc(sizeof(int) * LIST_SIZE);
	int *B = (int*) malloc(sizeof(int) * LIST_SIZE);

	for(int i=0;i<LIST_SIZE;i++){
		scanf("%d",&A[i]);
	}

	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint n_devices;
	cl_uint n_platforms;

	clGetPlatformIDs(1,&platform_id,&n_platforms);

	cl_int ret = clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_CPU,1,&device_id,&n_devices);
	if (ret != CL_SUCCESS) {
			printf("Error: Failed to get Device ID!\n");
			return EXIT_FAILURE;
		}

	cl_context context = clCreateContext(NULL,1,&device_id,NULL,NULL,&ret);
	if (!context) {
				printf("Error: Failed to create context !\n");
				return EXIT_FAILURE;
			}

	cl_command_queue command_queue = clCreateCommandQueue(context,device_id,CL_QUEUE_PROFILING_ENABLE,&ret);

	if (!command_queue) {
					printf("Error: Failed to create command queue!\n");
					return EXIT_FAILURE;
				}

	cl_mem a = clCreateBuffer(context,CL_MEM_READ_WRITE,LIST_SIZE*sizeof(int),NULL,&ret);
	if (!a) {
					printf("Error: Failed to create buffer a!\n");
					return EXIT_FAILURE;
				}
	cl_mem b = clCreateBuffer(context,CL_MEM_READ_WRITE,LIST_SIZE*sizeof(int),NULL,&ret);
	if (!b) {
					printf("Error: Failed to create buffer b!\n");
					return EXIT_FAILURE;
				}

	ret = clEnqueueWriteBuffer(command_queue,a,CL_TRUE,0,LIST_SIZE*sizeof(int),A,0,NULL,NULL);

	if (ret != CL_SUCCESS) {
					printf("Error: Failed to write buffer!\n");

					return EXIT_FAILURE;
				}

	cl_program program = clCreateProgramWithSource(context,1,(const char**)&stream,NULL,&ret);

	if (!program) {
					printf("Error: Failed to generate program with source!\n");
					return EXIT_FAILURE;
				}

	ret = clBuildProgram(program,1,&device_id,NULL,NULL,NULL);

	if (ret != CL_SUCCESS) {
					printf("Error: Failed to Build program!\n");
					printf("%d\n",ret);
					return EXIT_FAILURE;
}

	cl_kernel kernel= clCreateKernel(program,"selection_sort",&ret);

	if (!kernel) {
					printf("Error: Failed to create kernel!\n");
					return EXIT_FAILURE;
				}


	ret = clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&a);
	if (ret != CL_SUCCESS) {
						printf("Error: Failed to set arg1!\n");
						return EXIT_FAILURE;
					}


	ret = clSetKernelArg(kernel,1,sizeof(cl_mem),(void*)&b);

	if (ret != CL_SUCCESS) {
						printf("Error: Failed to set arg 2\n");
						return EXIT_FAILURE;
					}




	size_t global_item_size = LIST_SIZE;
	size_t local_item_size = 1;

	cl_event event;


		ret = clEnqueueNDRangeKernel(command_queue,kernel,1,NULL,&global_item_size,&local_item_size,0,NULL,&event);

		if (ret != CL_SUCCESS) {
							printf("Error: Failed NDrangeKernel\n");
							return EXIT_FAILURE;
						}




	ret = clFlush(command_queue);
	ret = clFinish(command_queue);

	printf("Input Array : ");
	for(int i=0;i<LIST_SIZE;i++)	printf("%d ",A[i]);

	ret = clEnqueueReadBuffer(command_queue,a,CL_TRUE,0,LIST_SIZE*sizeof(int),A,0,NULL,NULL);

	if (ret != CL_SUCCESS) {
						printf("Error: Failed to read buffer!\n");
						return EXIT_FAILURE;
					}


	printf("\nOutput Array : ");
	for(int i=0;i<LIST_SIZE;i++)	printf("%d ",B[i]);

	cl_ulong start = 0,end = 0;

	ret = clGetEventProfilingInfo(event,CL_PROFILING_COMMAND_START,sizeof(start),&start,NULL);
	ret = clGetEventProfilingInfo(event,CL_PROFILING_COMMAND_END,sizeof(end),&end,NULL);

	/*printf("\nTime taken to execute kernel : %d",(double)(end-start));*/



	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(A);
	free(B);

	return 0;
}
