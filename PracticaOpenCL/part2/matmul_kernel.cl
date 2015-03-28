/////////////////////////////////////////////////////////////////////
// Matrix multiplication - Kernel code [Corresponding to part 3]
// ------------------------------------------------------------------
// OpenCL Kernel, using as many WorkItem as we've got in 'x' 
// dimension.
/////////////////////////////////////////////////////////////////////  


/* Matrix dimensions are assumed to be multiples of BLOCK_SIZE */
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
__kernel void MatMulKernel(__global float *A, __global float *B, __global float *C){

  int idX = get_global_id(0);
  int idY = get_global_id(1);
  int sizeX = get_global_size(0);

  /* Codi de multiplicacio de les matrius A * B.*/
  for (int y=0; y<sizeX; y++){
    C[(y*sizeX)+idX] = doAPoint(idX, y, A, B, sizeX, sizeX);
  }

}
