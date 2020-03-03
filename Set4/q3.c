
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>



int main (int argc, char* argv[]) {

	int rank, size,error;
	
	int mat2[50],sum = 0;
	int mat[50][50],mat1[50];
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank==0) {
		printf("Enter a 4x4 matrix\n");
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				scanf("%d", &mat[i][j]);
			}
		}
		printf("OUTPUT MATRIX\n");
	}

	error=MPI_Scatter(mat,50,MPI_INT,mat1,50,MPI_INT,0,MPI_COMM_WORLD);
	if(error!=MPI_SUCCESS){
		printf("Coudn't scatter. Exiting...");
		exit(1);
	}
	error=MPI_Scan(mat1,mat2,4,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	if(error!=MPI_SUCCESS){
		printf("Coudn't scan. Exiting...");
		exit(1);
	}

	for(int i = 0; i < 4; i++){
		printf("%d ", mat2[i]);
	}
	printf("\n");

	

	
	MPI_Finalize();

	return 0;
}