/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  //printf("transpose submit called\n");
  int i, j, row, t1, t2, t3, t4, t5, t6, t7, t8;
  if(M == 32 && N == 32){
    for(i = 0; i < N; i += 8){
      for(j = 0; j < N; j += 8){
        for(row = 0; row < 8; row++){
	
          t1 = A[i + row][j];
          t2 = A[i+ row][j + 1];
          t3 = A[i + row][j + 2];
          t4 = A[i + row][j + 3];
          t5 = A[i + row][j + 4];
          t6 = A[i + row][j + 5];
          t7 = A[i + row][j + 6];
          t8 = A[i + row][j + 7];

          B[j][i + row] = t1;
          B[j + 1][i + row] = t2;
          B[j + 2][i + row] = t3;
          B[j + 3][i + row] = t4;
          B[j + 4][i + row] = t5;
          B[j + 5][i + row] = t6;
          B[j + 6][i + row] = t7;
          B[j + 7][i + row] = t8;
        }
      }
    }
  }
  else if(M == 61 && N == 67){
    for(i = 0; i < 64; i += 8){
      for(j = 0; j < 56; j += 8){
        for(row = 0; row < 8; row++){
	
          t1 = A[i + row][j];
          t2 = A[i+ row][j + 1];
          t3 = A[i + row][j + 2];
          t4 = A[i + row][j + 3];
          t5 = A[i + row][j + 4];
          t6 = A[i + row][j + 5];
          t7 = A[i + row][j + 6];
          t8 = A[i + row][j + 7];

          B[j][i + row] = t1;
          B[j + 1][i + row] = t2;
          B[j + 2][i + row] = t3;
          B[j + 3][i + row] = t4;
          B[j + 4][i + row] = t5;
          B[j + 5][i + row] = t6;
          B[j + 6][i + row] = t7;
          B[j + 7][i + row] = t8;
        }
      }
    }
    for(i = 64, j = 0; j < 56; j += 8){//left underneath
      for(row = 0; row < 3; row++){
	t1 = A[i + row][j];
        t2 = A[i + row][j + 1];
        t3 = A[i + row][j + 2];
        t4 = A[i + row][j + 3];
        t5 = A[i + row][j + 4];
        t6 = A[i + row][j + 5];
	t7 = A[i + row][j + 6];
        t8 = A[i + row][j + 7];

        B[j][i + row] = t1;
        B[j + 1][i + row] = t2;
        B[j + 2][i + row] = t3;
        B[j + 3][i + row] = t4;
        B[j + 4][i + row] = t5;
	B[j + 5][i + row] = t6;
        B[j + 6][i + row] = t7;
        B[j + 7][i + row] = t8;
      }
    }
    for(i = 0, j = 56; i < M; i++){//left to the right
      t1 = A[i][j];
      t2 = A[i][j + 1];
      t3 = A[i][j + 2];
      t4 = A[i][j + 3];
      t5 = A[i][j + 4];

      B[i][j] = t1;
      B[i][j + 1] = t2;
      B[i][j + 2] = t3;
      B[i][j + 3] = t4;
      B[i][j + 4] = t5;
    }
  }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
  //printf("registerFunctions start\n");
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

    //printf("registerFunctions end\n");
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

