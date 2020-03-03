#include<stdio.h>
#include<string.h>
#include<mpi.h>

int main(int argc,char *argv[]){
	int rank,size,n,len,j,len1;
	char str1[20],str2[20],recstr1[20],recstr2[20],str3[40],finalstr[40];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	if(rank==0){
		printf("Enter two strings of equal lengths(Length should be evenly divisible by %d)\n",size);
		gets(str1);
		gets(str2);
		len= strlen(str1);
		n=len/size;	
	}

	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(str1,n,MPI_CHAR,recstr1,n,MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Scatter(str2,n,MPI_CHAR,recstr2,n,MPI_CHAR,0,MPI_COMM_WORLD);

	printf("Received %s in process %d \n",recstr1,rank);
	printf("Received %s in process %d \n",recstr2,rank);
	j=0;
	for(int i=0;i<(2*n);i++){
		str3[j]=recstr1[i];
		str3[j+1]=recstr2[i];
		j=j+2;
	}
	len1=strlen(str3);
	printf("Combined string in process %d : %s\n",rank,str3);


	MPI_Gather(str3,len1,MPI_CHAR,finalstr,len1,MPI_CHAR,0,MPI_COMM_WORLD);

	if(rank==0){
		printf("Resultant string : %s\n",finalstr);
	}

	MPI_Finalize();
}