#include <stdio.h>
#include <CL/cl.h>
#include <string.h>
/*#include <conio.h>*/
#include <stdlib.h>
#include <time.h>
#define MAX_SOURCE_SIZE 0x100000

int main(){

	time_t start,end;
	start=clock();

	char tempstr[20486];

	int i;
	/*for(i=0;i<20485;i++){
		tempstr[i]='A';
	}
	tempstr[20486]='\0';

	int len=strlen(tempstr);
	len++;*/

	char *str=(char*)malloc(sizeof(char)*20);
	printf("Enter a string\n");
	/*strcpy(str,tempstr);*/
	gets(str);
	puts(str);
	int len=strlen(str);
	printf("Length of the string = %d",len);

	FILE *kernel_code_file = fopen("samplekernel.cl", "r");
		if (kernel_code_file == NULL) {
			printf("Kernel loading failed.");
			exit(1);
		}

		char *source_str = (char *)malloc(MAX_SOURCE_SIZE);

		size_t source_size = fread(source_str, 1, MAX_SOURCE_SIZE, kernel_code_file);
		printf("No. of bytes read from kernel file = %d\n",source_size);
		fclose(kernel_code_file);

		// 2. Get platform and device information

			cl_platform_id platform_id = NULL;
			cl_device_id device_id = NULL;

			cl_uint ret_num_devices, ret_num_platforms;

			cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
			ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_CPU, 1, &device_id, &ret_num_devices);

		// 3. Create an OpenCL context

			cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);


		// 4. Create a command queue

			cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);


		// 5. Create memory buffers on the device for each vector A

			cl_mem s_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, len * sizeof(char), NULL, &ret);

			cl_mem t_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, len * sizeof(char), NULL, &ret);


			ret= clEnqueueWriteBuffer(command_queue,s_mem_obj,CL_TRUE,0,len*sizeof(char),str,0,NULL,NULL);

			cl_program program=clCreateProgramWithSource(context,1,(const char **)&source_str,(const size_t*)&source_size,&ret);


			ret = clBuildProgram(program,1,&device_id,NULL,NULL,NULL);

			cl_kernel kernel = clCreateKernel(program,"str_chgcase",&ret);

			ret=clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&s_mem_obj);
			ret=clSetKernelArg(kernel,1,sizeof(cl_mem),(void*)&t_mem_obj);

			size_t global_item_size =len;

			size_t local_item_size=1;

			cl_event event;

			ret =clEnqueueNDRangeKernel(command_queue,kernel,1,NULL,&global_item_size,&local_item_size,0,NULL,&event);

			ret=clFinish(command_queue);

			cl_ulong time_start,time_end;
			double total_time;

			clGetEventProfilingInfo(event,CL_PROFILING_COMMAND_END,sizeof(time_end),&time_end,NULL);

			total_time = (double)(time_end-time_start);

			char *strres = (char*)malloc(sizeof(char)*len);
			ret=clEnqueueReadBuffer(command_queue,t_mem_obj,CL_TRUE,0,len*sizeof(char),strres,0,NULL,NULL);

			printf("\nDone\n");
			strres[len]='\0';
			printf("Resultant toggled string : %s\n",strres);
			getchar();

			ret= clFlush(command_queue);
			ret=clReleaseKernel(kernel);
			ret=clReleaseProgram(program);
			ret=clReleaseMemObject(t_mem_obj);
			ret=clReleaseMemObject(s_mem_obj);
			ret=clReleaseCommandQueue(command_queue);
			ret=clReleaseContext(context);

			end=clock();
			printf("Time taken to execute the Kernel in ms = %0.3fmsec\n\n",total_time/1000000);
			printf("Time taken to execute the whole program in seconds = %0.3f Seconds\n\n",(end-start)/(double)CLOCKS_PER_SEC);

			free(str);
			free(strres);

			return 0;































}

