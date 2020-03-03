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
	fp = fopen("q3kernel.cl","r");

	stream = (char*) malloc(sizeof(char)*MAX_SIZE);

	size = fread(stream,sizeof(char),MAX_SIZE,fp);

	fclose(fp);

	int *A = (int*) malloc(sizeof(int) * LIST_SIZE);

	for(int i=0;i<LIST_SIZE;i++){
		scanf("%d",&A[i]);
	}

	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint n_devices;
	cl_uint n_platforms;

	clGetPlatformIDs(1,&platform_id,&n_platforms);

	cl_int ret = clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_CPU,1,&device_id,&n_devices);

	cl_context context = clCreateContext(NULL,1,&device_id,NULL,NULL,&ret);

	cl_command_queue command_queue = clCreateCommandQueue(context,device_id,CL_QUEUE_PROFILING_ENABLE,&ret);

	cl_mem a = clCreateBuffer(context,CL_MEM_READ_WRITE,LIST_SIZE*sizeof(int),NULL,&ret);

	ret = clEnqueueWriteBuffer(command_queue,a,CL_TRUE,0,LIST_SIZE*sizeof(int),A,0,NULL,NULL);

	cl_program program = clCreateProgramWithSource(context,1,(const char**)&stream,NULL,&ret);

	ret = clBuildProgram(program,1,&device_id,NULL,NULL,NULL);

	cl_kernel kernel_odd = clCreateKernel(program,"sortArrayOdd",&ret);
	cl_kernel kernel_even = clCreateKernel(program,"sortArrayEven",&ret);

	ret = clSetKernelArg(kernel_odd,0,sizeof(cl_mem),(void*)&a);

	ret = clSetKernelArg(kernel_even,0,sizeof(cl_mem),(void*)&a);

	size_t global_item_size = LIST_SIZE/2;
	size_t local_item_size = 1;

	cl_event event;

	for(int i=0;i<LIST_SIZE;i++){
		ret = clEnqueueNDRangeKernel(command_queue,kernel_odd,1,NULL,&global_item_size,&local_item_size,0,NULL,&event);
		ret = clEnqueueNDRangeKernel(command_queue,kernel_even,1,NULL,&global_item_size,&local_item_size,0,NULL,&event);
	}



	ret = clFlush(command_queue);
	ret = clFinish(command_queue);

	printf("Input Array : ");
	for(int i=0;i<LIST_SIZE;i++)	printf("%d ",A[i]);

	ret = clEnqueueReadBuffer(command_queue,a,CL_TRUE,0,LIST_SIZE*sizeof(int),A,0,NULL,NULL);

	printf("\nOutput Array : ");
	for(int i=0;i<LIST_SIZE;i++)	printf("%d ",A[i]);

	cl_ulong start = 0,end = 0;

	ret = clGetEventProfilingInfo(event,CL_PROFILING_COMMAND_START,sizeof(start),&start,NULL);
	ret = clGetEventProfilingInfo(event,CL_PROFILING_COMMAND_END,sizeof(end),&end,NULL);

	/*printf("\nTime taken to execute kernel : %d",(double)(end-start));*/


	ret = clReleaseKernel(kernel_odd);
	ret = clReleaseKernel(kernel_even);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(A);

	return 0;
}
