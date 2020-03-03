#include<mpi.h>
#include<stdio.h>
#include<string.h>


int main(int argc,char *argv[]){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int buffer[100],buffer1[100];
	int len=100;
	MPI_Buffer_attach(buffer,len);
	int arr[10];
	int r;
	MPI_Status status;
	/*memset(buffer,len, 0);*/


	if(rank==0){
	
		printf("Enter %d numbers.\n",size-1);
		for(int i=0;i<size-1;i++){
			scanf("%d",&buffer1[i]);
		}
		int count=0;
	
		for(int i=0;i<size-1;i++){
			MPI_Bsend(&buffer1[count],1,MPI_INT,i+1,i+1,MPI_COMM_WORLD);
			count++;
		}
		

	}
	else{

		if(rank%2==0){
			MPI_Recv(&r,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		printf("I have received %d in process %d\n",r,rank);
		printf("Sqaure of %d = %d\n",r,r*r);
		}
		else{
			MPI_Recv(&r,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		printf("I have received %d in process %d\n",r,rank);
		printf("Cube of %d = %d\n",r,r*r*r);
		}
		
	}
	


		MPI_Buffer_detach(&buffer,&len);
		MPI_Finalize();
		


}