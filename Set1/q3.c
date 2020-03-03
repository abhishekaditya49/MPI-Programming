#include<mpi.h>
#include<stdio.h>

int main(int argc,char *argv[]){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank==0)
		printf("10 + 5 =%d\n",10+5);
	else if(rank==1)
		printf("10 - 5 =%d\n",10-5);
	else if(rank==2)
		printf("10 * 5 = %d\n",10*5);
	else printf("10 / 2 = %d\n",10/2);

	MPI_Finalize();
	return 0;

}