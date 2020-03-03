__kernel void oneCompliment(__global float* A, __global float* B, __global float* C)
{
    int idx = get_global_id(0);
    int temp = A[idx];
    int res=0,j=0;
    int ele=0;
    while(temp!=0){
    B[j++]=((temp%10)+1)%2;
    temp = temp/10;
    }
    j--;
    for(;j>=0;j--){
    res = res*10 + B[j];
    }
    C[idx]=res;
}