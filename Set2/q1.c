#include<mpi.h>
#include<stdio.h>
#include<string.h>


int main(int argc,char *argv[]){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;
	char buf[10];
	char buf1[10];
	int count;
	if(rank==0){
		printf("Enter a word.\n");
		gets(buf);
		/*count=strlen(buf);*/
		MPI_Ssend(buf,10,MPI_CHAR,1,1,MPI_COMM_WORLD);
		printf("I have sent %s from process %d\n",buf,rank);


		MPI_Recv(buf1,10,MPI_CHAR,1,1,MPI_COMM_WORLD,&status);

		printf("I have received [TOGGLED]%s in process %d\n",buf1,rank);

	}
	else {

		MPI_Recv(buf,10,MPI_CHAR,0,1,MPI_COMM_WORLD,&status);

		printf("I have received %s  in %d\n",buf,rank);

		int len = strlen(buf);
		for(int i=0;i<len;i++){
			if(buf[i] >= 65 && buf[i] <= 90)
				buf[i]= buf[i]+32;
			else
				buf[i]=buf[i]-32;
		}

		MPI_Ssend(buf,10,MPI_CHAR,0,1,MPI_COMM_WORLD);

	}
	MPI_Finalize();
	return 0;

}
