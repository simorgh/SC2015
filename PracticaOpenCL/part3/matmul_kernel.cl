/////////////////////////////////////////////////////////////////////
// Matrix multiplication - Kernel code [Optional]
// ------------------------------------------------------------------
// OpenCL Kernel, using as many WorkItems as elements on the matrix.
/////////////////////////////////////////////////////////////////////  


/* Matrix dimensions are assumed to be multiples of BLOCK_SIZE */
float doAPoint( __private int x, __private int y, __global float* A,__global float *B,
 __private const int sizeAX, __private const int sizeBX) {
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
__kernel void MatMulKernel(__global float *A, __global float *B, __global float *C){

  int idX = get_global_id(0);
  int idY = get_global_id(1);
  int sizeX = get_global_size(0);
  int myGlobalDataPosition = idY*sizeX + idX;

  /* A*B matrix multiplication code. Each workItem computes it's result for an element on matrix C */
  float result = doAPoint(idX, idY, A, B, sizeX, sizeX);
  C[ myGlobalDataPosition ] = result;

}
