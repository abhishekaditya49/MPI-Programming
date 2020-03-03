#include<mpi.h>
#include<stdio.h>

void printprime(int a,int b){
	int count;
	while(a<=b){
		count=0;
		for(int i=1;i<=a/2;i++){
			if(a%i==0)
				count++;
		}

		if(count<=1){
			printf("%d ",a);
		}
		a++;
	}
}

int main(int argc,char *argv[]){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(rank==0){
		printprime(1,50);
	}
	else printprime(51,100);
	MPI_Finalize();
	return 0;

}

