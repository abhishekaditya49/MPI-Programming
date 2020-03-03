__kernel void selection_sort(__global int * A,__global int * B)
{
  int id = get_global_id(0); // current thread
  int n = get_global_size(0); // input size
 int pos=0,i;
 printf("A[id] =%d\n",A[id]);
 for(i=0;i<n;i++){
 if(A[i]<A[id] || A[i]==A[id] && i<id){
 	pos++;
 }
 B[pos]=A[id];
 printf("B[pos] =%d\n",B[pos]);
 
}