#include <stdio.h>
#include <string.h>
#include <mpi.h>




int main (int argc, char *argv[]) {

	int rank, size;
	float x,y,a,pi;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	x=(float)(rank+1)/size;
	y=4.f/(1+x*x);
	a=(1/(float)size)*y;

	MPI_Reduce(&a, &pi, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank==0){
		printf("%f\n", pi);
	}
	
	MPI_Finalize();

	return 0;
}