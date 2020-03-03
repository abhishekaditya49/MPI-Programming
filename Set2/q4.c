#include<mpi.h>
#include<stdio.h>
#include<string.h>


int main(int argc,char *argv[]){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;
	int x;
	if(rank==0){
		printf("Enter a number.\n");
		scanf("%d",&x);
		x++;
		MPI_Ssend(&x,1,MPI_INT,rank+1,rank+1,MPI_COMM_WORLD);
		printf("I have send %d from process %d\n",x,rank);

		MPI_Recv(&x,1,MPI_INT,size-1,size,MPI_COMM_WORLD,&status);
		printf("I have received %d in process%d from process%d\n",x,rank,size-1);
	}
	else {

		MPI_Recv(&x,1,MPI_INT,rank-1,rank,MPI_COMM_WORLD,&status);
		printf("I have received %d in process%d from process%d\n",x,rank,rank-1);

		if(rank<size-1){
			x++;
			MPI_Ssend(&x,1,MPI_INT,rank+1,rank+1,MPI_COMM_WORLD);

		}
		else{
			x++;
			MPI_Ssend(&x,1,MPI_INT,0,size,MPI_COMM_WORLD);
		}

		/*printf("I have received %d  in %d\n",x,rank);*/

		
	}
	MPI_Finalize();
	return 0;

}
