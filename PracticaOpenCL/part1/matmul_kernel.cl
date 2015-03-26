/* Matrix multiplication - Kernel code
 * Matrix dimensions are assumed to be multiples of BLOCK_SIZE */
float doAPoint( __private int x, __private int y, __global float* A,__global float *B, __private const int sizeAX, __private const int sizeBX) {
  /* We assume sizeAX == sizeBY */
  __private float result=0;
  __private int posA, posB;

  for (int step=0; step<sizeAX; step++){ 
    posA = (y*sizeAX) + step ;
    posB = (step*sizeBX) + x;
    result = result + A[posA] *B[posB];
  }

  return result;
}

/* Matrix multiplication kernel */
__kernel void MatMulKernel(int size, __global float *A, __global float *B, __global float *C){

  /* Codi de multiplicacio de les matrius A * B.*/
  for (int y=0; y<size; y++){
    for (int x=0; x<size; x++)
      C[(y*size)+x] = doAPoint(x, y, A, B, size, size);
  }

  //barrier(CLK_GLOBAL_MEM_FENCE);

}
