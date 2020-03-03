__kernel void octalconversion(__global int *A,
						 __global int *B) {
int i= get_global_id(0);

int a = A[i];
int b = 0, temp = 1;

while (a!=0){
	b = b + (a % 8) * temp;
    a = a / 8;
    temp = temp * 10;
    }
B[i]=b;
}