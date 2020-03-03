#include<stdio.h>
#include<mpi.h>

int main(int argc,char argv[]){
	int rank,size;
	int fact=1,factsum;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	

	for(int i=1;i<=rank+1;i++){
		fact =fact*i;
	}

	MPI_Scan(&fact,&factsum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

	if(rank==size-1){
	printf("Sum of first %d factorials = %d.\n",size,factsum);
	}

	MPI_Finalize();

}