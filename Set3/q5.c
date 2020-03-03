#include<stdio.h>
#include<string.h>
#include<mpi.h>

int main(int argc,char *argv[]){
	int rank,size,n,even=0,odd=0;
	int arr[50],arr1[50],arr2[50];
	

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	if(rank==0){
		printf("Enter size of array(Size of array should be divisible %d)\n",size);
		scanf("%d",&n);
		printf("Enter %d elements\n",n);
		for(int i=0;i<n;i++){
			scanf("%d",&arr[i]);
		}

		printf("Input Array (A): ");
		for(int i=0;i<n;i++){
			printf("%d ",arr[i]);
		}
		printf("\n");
	}
	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(arr,4,MPI_INT,arr1,4,MPI_INT,0,MPI_COMM_WORLD);

	for(int i=0;i<4;i++){
		if(arr1[i]%2==0){
			even++;
			arr1[i]=1;
		}
			
		
		else {
			odd++;
			arr1[i]=0;
		}
	}
	/*printf("Array after operation : ");
	for(int i=0;i<4;i++){
		printf("%d ",arr1[i]);
	}
	printf("\n");*/

	MPI_Gather(arr1,4,MPI_INT,arr2,4,MPI_INT,0,MPI_COMM_WORLD);

	if(rank==0){

		printf("Output Array (A): ");
		for(int i=0;i<n;i++){
			printf("%d ",arr2[i]);
		}
		printf("\n");
		printf("Even(Count) = %d\n",even);
		printf("Odd(Count) = %d\n",odd);
	}

	MPI_Finalize();
}