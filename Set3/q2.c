#include<stdio.h>
#include<mpi.h>

int average(int arr[],int n){
	int sum=0;
	int avg;
	for(int i=0;i<n;i++){
		sum=sum+arr[i];
	}
	avg=sum/n;
	return avg;
}

int main(int argc,char *argv[]){
	int rank,size,n,m,arr[100],rec[20],avg,avgarr[100],c;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank==0){
		n=size;
		printf("Enter m\n");
		scanf("%d",&m);
		printf("Enter %d elements:\n",n*m);
			for(int i=0;i<n*m;i++){
			scanf("%d",&arr[i]);
			}
		}
	MPI_Bcast(&m,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(arr,m,MPI_INT,rec,m,MPI_INT,0,MPI_COMM_WORLD);

	printf("Array received in process %d:\n",rank);
	for(int i=0;i<m;i++){
		printf("%d\t",rec[i]);
	}
	printf("\n");
	c=average(rec,m);
	MPI_Gather(&c,1,MPI_INT,avgarr,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank==0){
		printf("Avg array gathered in the root \n");

		for(int i=0;i<n;i++){
			printf("%d\t",avgarr[i]);
		}
		printf("\n");
	}
	MPI_Finalize();
	return 0;
}