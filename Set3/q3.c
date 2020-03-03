#include<stdio.h>
#include<string.h>
#include<mpi.h>

int main(int argc,char *argv[]){
	int rank,size,len,n,count,vowels[100];
	char str[20],recstr[20];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);


	if(rank==0){
		printf("Enter a string whose length is evenly divisible by %d\n",size);
		gets(str);
		len= strlen(str);
		n=len/size;		
		}


	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(str,n,MPI_CHAR,recstr,n,MPI_CHAR,0,MPI_COMM_WORLD);


	printf("Received string in process %d:",rank);
	printf("%s\n",recstr);

	count=0;
	for(int i=0;i<n;i++){
		if(recstr[i]=='a'||recstr[i]=='e'||recstr[i]=='i'||recstr[i]=='o'||recstr[i]=='u'||
			recstr[i]=='A'||recstr[i]=='E'||recstr[i]=='I'||recstr[i]=='O'||recstr[i]=='U'){
			continue;
		}
		else count++;
	}

	printf("Process No = %d,No. of non-vowels =%d\n",rank,count);
	MPI_Gather(&count,1,MPI_INT,vowels,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank==0){
		int total=0;
		for(int i=0;i<size;i++){
			printf("no. of no vowels in process %d = %d\n",i,vowels[i]);
			total=total+vowels[i];
		}
		printf("Total no of vowels in the string '%s' = %d\n",str,total);
	}

	MPI_Finalize();

}