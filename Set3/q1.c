#include<stdio.h>
#include<mpi.h>

int factorial(int n){
	if(n==0|| n==1)
		return n;
	else return n*factorial(n-1);
}

int main(int argc,char *argv[]){
	int rank,size,N,A[10],B[10],c;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	if(rank==0){
		N=size;
		printf("Enter %d values:\n",N);
		for(int i=0;i<N;i++){
			scanf("%d",&A[i]);
		}
	}
	MPI_Scatter(A,1,MPI_INT,&c,1,MPI_INT,0,MPI_COMM_WORLD);
	printf("I have received %d in process %d\n",c,rank);

	c=factorial(c);
	MPI_Gather(&c,1,MPI_INT,B,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank==0){
		printf("The result gathered in the root \n");
		for(int i=0;i<N;i++){
			printf("%d\t",B[i]);
		}
		printf("\n");
	}
	MPI_Finalize();
	return 0;
}

