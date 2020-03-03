#include<stdio.h>
#include<string.h>
#include<mpi.h>

int main(int argc,char *argv[]){
	int rank,size,n;
	int arr[50],arr1[50],arr2[50];
	

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	if(rank==0){
		printf("Enter size of array(Should be divisible by 4)\n");
		scanf("%d",&n);
		printf("Enter %d elements\n",n);
		for(int i=0;i<n;i++){
			scanf("%d",&arr[i]);
		}

		printf("Input Array : ");
		for(int i=0;i<n;i++){
			printf("%d ",arr[i]);
		}
		printf("\n");
	}
	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(arr,4,MPI_INT,arr1,4,MPI_INT,0,MPI_COMM_WORLD);

	for(int i=1;i<4;i++){
		arr1[i]=arr1[i]+arr1[i-1];
	}
	printf("Array after operation : ");
	for(int i=0;i<4;i++){
		printf("%d ",arr1[i]);
	}
	printf("\n");

	MPI_Gather(arr1,4,MPI_INT,arr2,4,MPI_INT,0,MPI_COMM_WORLD);

	if(rank==0){

		printf("Output Array : ");
		for(int i=0;i<n;i++){
			printf("%d ",arr2[i]);
		}
		printf("\n");
	}

	MPI_Finalize();
}