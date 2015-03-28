

/* Matrix multiplication kernel */
__kernel void MatMulKernel(int MATRIX_SIZE, int BLOCK_SIZE, __global float *A, __global float *B, __global float *C, __local float *BA, __local float *BB){
  /* We assume BLOCK_SIZE_H == BLOCK_SIZE_V */
/*
  // Block index
  int bx = get_group_id(0);
  int by = get_group_id(1);

  // Thread index
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

  float Csub;
  // Loop over all the sub-matrices of A and B required to compute the block sub-matrix
  for (int a=aBegin, b=bBegin; a <= aEnd; a += aStep, b += bStep){

      // Load the matrices from global memory to local memory; each thread loads one element of each matrix
      BA[ty][tx] = A[a + MATRIX_SIZE * ty + tx];
      BB[ty][tx] = B[b + MATRIX_SIZE * ty + tx];

      // Synchronize to make sure the matrices are loaded
      barrier(CLK_LOCAL_MEM_FENCE);
      // Multiply the two matrices together; each thread computes one element of the block sub-matrix
      for (int k = 0; k < BLOCK_SIZE; ++k) Csub += BA[ty][k] * BB[k][tx];
      // Synchronize to make sure that the preceding computation is done before loading two new sub-matrices of A and B in the next iteration
      barrier(CLK_LOCAL_MEM_FENCE);
  }

  // Write the block sub-matrix to device memory; each thread writes one element
  int c = MATRIX_SIZE * BLOCK_SIZE * by + BLOCK_SIZE * bx;
  C[c + MATRIX_SIZE * ty + tx] = Csub;
*/

    //Identification of this workgroup
    int i = get_group_id(0);
    int j = get_group_id(1);

    //Identification of work-item
    int idX = get_local_id(0);
    int idY = get_local_id(1);

    //matrixes dimensions
    int p = get_global_size(0);
    int r = get_global_size(1);

    int qq = 1;

    //Number of submatrixes to be processed by each worker (Q dimension)
    int numSubMat = qq/BLOCK_SIZE;

    float4 resp = (float4)(0,0,0,0);
    //__local float BA[BLOCK_SIZE][BLOCK_SIZE];
    //__local float BB[BLOCK_SIZE][BLOCK_SIZE];

    for (int k=0; k<numSubMat; k++)
    {
        //Copy submatrixes to local memory. Each worker copies one element
        //Notice that A[i,k] accesses elements starting from M[BLOCK_SIZE*i, BLOCK_SIZE*j]
        BA[idX][idY] = A[BLOCK_SIZE*i + idX + p*(BLOCK_SIZE*k+idY)];
        BB[idX][idY] = B[BLOCK_SIZE*k + idX + qq*(BLOCK_SIZE*j+idY)];
        barrier(CLK_LOCAL_MEM_FENCE);

        for (int k2 = 0; k2 < BLOCK_SIZE; k2+=4){
            float4 temp1=(float4) ( BA[idX][k2],BA[idX][k2+1],BA[idX][k2+2],BA[idX][k2+3] );
            float4 temp2=(float4) ( BB[k2][idY],BB[k2+1][idY],BB[k2+2][idY],BB[k2+3][idY] );
            resp += temp1 * temp2;
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    C[BLOCK_SIZE*i + idX + p*(BLOCK_SIZE*j+idY)] = resp.x+resp.y+resp.z+resp.w;

}
