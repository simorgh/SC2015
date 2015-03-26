

/*float doAPoint(__global int x, __global int y, __global float* A, __global float *B, __global const int sizeAX, __global const int sizeBX);*/
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
__kernel void MatMulKernel(__global float *A, __global float *B, __global float *C){
/* __local float *BA, __local float *BB){*/

  int idX = get_global_id(0);
  int idY = get_global_id(1);
  int sizeX = get_global_size(0);
  int myGlobalDataPosition = idY*sizeX + idX;

  /* Codi de multiplicacio de les matrius A * B.
   Cada Work Item s'encarrega del resultat d'un punt de la matriu C.*/
  float result = doAPoint(idX, idY, A, B, sizeX, sizeX);
  C[ myGlobalDataPosition ] = result;

  // Calculant resultats a GPU 
  for (int y=0; y<MATRIX_SIZE; y++){
    for (int x=0; x<MATRIX_SIZE; x++) C[(y*sizeX)+x] = doAPoint(idX, idY, A, B, sizeX, sizeX);
  }

}
