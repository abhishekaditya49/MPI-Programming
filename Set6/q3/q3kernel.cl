__kernel void sortArrayEven(__global int* A){						
	int id = get_global_id(0)*2;
	
	int temp = A[id];

	if(A[id] > A[id+1]){
		A[id] = A[id+1];
		A[id+1] = temp;
	}

}

__kernel void sortArrayOdd(__global int* A){						
	int id = get_global_id(0)*2 + 1;

	int temp = A[id];

	if(A[id] > A[id+1]){
		A[id] = A[id+1];
		A[id+1] = temp;;
	}

}
