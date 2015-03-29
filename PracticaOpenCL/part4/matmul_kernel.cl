///////////////////////////////////////////////////////////////////////////
// Matrix multiplication - Kernel code [Corresponding to part 4]
// ------------------------------------------------------------------------
// OpenCL Kernel, calculation with __local:
// This is the main point of this case study because it shows how we can 
// take advantage of workgroups by using barrier and __local variables.
// What we want at this point is to split the matrix multiplication
// problem into submatrixes that fit in the local workgroup size.  
//////////////////////////////////////////////////////////////////////////


/* Matrix multiplication kernel */
__kernel void MatMulKernel( int MATRIX_SIZE, int BLOCK_SIZE, __global float *A, __global float *B, 
    __global float *C, __local float *BA, __local float *BB){
  /* We assume BLOCK_SIZE_H == BLOCK_SIZE_V */

  // Block index (identification of this workgroup)
  int bx = get_group_id(0);
  int by = get_group_id(1);

  // Thread index (identification of work-item)
  int tx = get_local_id(0);
  int ty = get_local_id(1);

  // Index of the first sub-matrix of A processed by the block
  int aBegin = MATRIX_SIZE * BLOCK_SIZE * by;
  // Index of the last sub-matrix of A processed by the block
  int aEnd   = aBegin + MATRIX_SIZE - 1;
  // Step size used to iterate through the sub-matrices of A
  int aStep  = BLOCK_SIZE;
  // Index of the first sub-matrix of B processed by the block
  int bBegin = BLOCK_SIZE * bx;
  // Step size used to iterate through the sub-matrices of B
  int bStep  = BLOCK_SIZE * MATRIX_SIZE;

  float Csub = 0.;
  // Loop over all the sub-matrices of A and B required to compute the block sub-matrix
  for (int a=aBegin, b=bBegin; a <= aEnd; a += aStep, b += bStep){
      // Load the matrices from global memory to local memory; each thread loads one element of each matrix
      BA[a + MATRIX_SIZE * ty + tx] = A[a + MATRIX_SIZE * ty + tx];
      BB[b + MATRIX_SIZE * ty + tx] = B[b + MATRIX_SIZE * ty + tx];
      // Synchronize to make sure the matrices are loaded
      barrier(CLK_LOCAL_MEM_FENCE);
	  
      // Multiply the two matrices together; each thread computes one element of the block sub-matrix
      for (int k = 0; k < BLOCK_SIZE; ++k) Csub += BA[a + MATRIX_SIZE * ty + k] * BB[b + MATRIX_SIZE * k + tx];
	   // Synchronize to make sure that the preceding computation is done before loading two new sub-matrices of A and B in the next iteration
      barrier(CLK_LOCAL_MEM_FENCE);
  }

  // Write the block sub-matrix to device memory; each thread writes one element
  int c = MATRIX_SIZE * BLOCK_SIZE * by + BLOCK_SIZE * bx;
  C[c + MATRIX_SIZE * ty + tx] = Csub;
}
