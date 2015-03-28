/////////////////////////////////////////////////////////////////////
// Matrix multiplication - Host code [Corresponding to part 4]
// ------------------------------------------------------------------
// OpenCL Kernel, using just 1 WorkItem implements the same algorithm
// that we've got in standart C.  
/////////////////////////////////////////////////////////////////////  

/* Host code */
/* Matrices are stored in row-major order: */
/* M(row, col) = *(M.elements + row * M.width + col) */

#include <stdio.h>
#include "../simple-opencl/simpleCL.h"

#define BLOCK_SIZE_H 16
#define BLOCK_SIZE_V 16
#define ALPHA 0.0001
#define MATRIX_SIZE 1024
#define DEVICE_ID 1

/* Matrix multiplication - Host code */
/* Matrix dimensions are assumed to be multiples of BLOCK_SIZE */
float doAPoint(int x, int y, float* A, float *B, const int sizeAX, const int sizeBX) {
	/* We assume sizeAX == sizeBY */
	float result=0;
	int posA,posB;
		
	for (int step=0; step<sizeAX; step++){
	  posA = (y*sizeAX) + step ;
	  posB = (step*sizeBX) + x;
	  result = result + A[posA] *B[posB];
	}
	return result;
}

int main() {
  /* This code executes on the OpenCL host */
  int found;

  /* SimpleOpenCL types declaration */
  sclHard* hardware;
  sclSoft software;

  /* Host data */
  float *A = NULL;      // Input array
  float *B = NULL;      // Input array
  float *C = NULL;      // Output array
  float *Ctest = NULL;  // Test array

  float diff;

  /* Size of the matrix */
  const int elements = MATRIX_SIZE * MATRIX_SIZE;

  /* Compute the size of the data */ 
  size_t datasize = sizeof(float) * elements;

  /* Allocate space for input/output data */
  A = (float *) malloc(datasize);
  B = (float *) malloc(datasize);
  C = (float *) malloc(datasize);
  Ctest = (float *) malloc(datasize);

  /* Initialize the input data */
  for(int i=0; i < elements; i++) {
    A[i] = i*0.00001;
    B[i] = 0.001;
  }

  /* NDRange 2D size initialization*/
  size_t global_size[2]; //NOTA:quants WorkItems hi ha en total entre els eixos de X->[0]/Y->[1]
  size_t local_size[2];
  size_t dataSize=sizeof(float)*elements;
  size_t localBlockSize = sizeof(float)*BLOCK_SIZE_H*BLOCK_SIZE_V;

  global_size[0]=MATRIX_SIZE; global_size[1]=MATRIX_SIZE;
  local_size[0]=BLOCK_SIZE_H; local_size[1]=BLOCK_SIZE_V;
  
  /* Init hardware & software */

  // Get the hardware
  hardware = sclGetAllHardware(&found);

  // Get the software
  software = sclGetCLSoftware( "matmul_kernel.cl", "MatMulKernel", hardware[DEVICE_ID] );

  /* Kernel execution */
  int size = MATRIX_SIZE;
  int bsize = BLOCK_SIZE_H;
  sclManageArgsLaunchKernel( hardware[DEVICE_ID], software, global_size, local_size,
                           " %a %a %r %r %w %N %N", sizeof(int), &size, sizeof(int), &bsize,
                           datasize, A, datasize, B, datasize, C,
                           sizeof(float)*BLOCK_SIZE_H*BLOCK_SIZE_V, 
                           sizeof(float)*BLOCK_SIZE_H*BLOCK_SIZE_V );

  /* Show results */
  printf("\nResultats GPU\n");
  for (int y=0; y<MATRIX_SIZE; y++){
    for (int x=0; x<MATRIX_SIZE; x++) {
      printf("%f\t", C[(y*MATRIX_SIZE)+x]); 
    }
    printf("\n");
  }
  
  /* Check results */
  printf("\nCalculant resultats a CPU...\n");
  for (int y=0; y<MATRIX_SIZE; y++){
  	for (int x=0; x<MATRIX_SIZE; x++) {
  	  Ctest[(y*MATRIX_SIZE)+x] = doAPoint(x,y,A,B,MATRIX_SIZE,MATRIX_SIZE);
  	}
  }

  int count = 0;
  for (int i=0; i<elements; i++){
  	diff=C[i]-Ctest[i];
  	if (diff>ALPHA){
  		count++;
   		printf("\nError a la posicio %d de C. Valor de C = %f. Valor de Ctest= %f.",i,C[i],Ctest[i]);
  	}
  }
  printf("\n");

  /*
    NOTA: Per comprovar el temps que triga la CPU podem o bé usar time (més 'fair' perque tindra en compta el 
    temps de transferencia, <temps d'accés o latència>) via CLI -->(cal comentar la part de HOST!). 
    O bé, usar la comanda cl_ulong sclManageArgsLaunchKernel (crear event, capturar el retorn i pasarli el event a 
    sclGetEventTime amb la variable HARDWARE utilitzada, ens retornara el temps que podrem imprimir per pantalla).
  */
  return 0;
}




