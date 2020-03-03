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
	int count;
	if(rank==0){
		printf("Enter a number.\n");
		scanf("%d",&x);
		/*count=strlen(buf);*/
		for(int i=1;i<size;i++){
			MPI_Send(&x,1,MPI_INT,i,1,MPI_COMM_WORLD);
		}
		printf("I have sent %d from process %d\n",x,rank);
	}
	else {

		MPI_Recv(&x,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);

		printf("I have received %d  in %d\n",x,rank);

		
	}
	MPI_Finalize();
	return 0;

}
