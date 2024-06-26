#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv) {  /* mpi_allgather.c  */
     long int N;
     int nthreads;
     long int i;
     double dot = 0.0;
     nthreads = atoi(argv[1]);
     N = atoi(argv[2]);

     int* a = (int*) malloc(sizeof(long int)*N);
     int* b = (int*) malloc(sizeof(long int)*N);

     for ( i=0; i<N; i++){
          a[i] = 2;
          b[i] = 5;
     }
     #pragma omp simd reduction (+:dot)
     for (i =0; i<N; i++){
          dot += a[i] + b[i];
     }


     clock_t start, end;
     end = clock();
     double time_taken = (double)(end - start)/ (double)CLOCKS_PER_SEC;
     free(a);
     free(b);
     printf("openmp simd 4 %9.3f  , %f \n", dot, time_taken);
  return(0);
}
