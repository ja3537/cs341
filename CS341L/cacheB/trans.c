/**********
 * Joseph Adams
 * jpadams
 * adamsjp@unm.edu
 *
 * Cache Lab part B
 * due 11/25/20
 **********/


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
  int i, j, row, col, t0, t1, t2, t3, t4, t5, t6, t7;

  //A very simplistic solution, store and transpose row by row
  if(M == 32 && N == 32){
    for(i = 0; i < N; i += 8){
      for(j = 0; j < N; j += 8){
        for(row = 0; row < 8; row++){
	
          t0 = A[i + row][j];
          t1 = A[i + row][j + 1];
          t2 = A[i + row][j + 2];
          t3 = A[i + row][j + 3];
          t4 = A[i + row][j + 4];
          t5 = A[i + row][j + 5];
          t6 = A[i + row][j + 6];
          t7 = A[i + row][j + 7];

          B[j][i + row] = t0;
          B[j + 1][i + row] = t1;
          B[j + 2][i + row] = t2;
          B[j + 3][i + row] = t3;
          B[j + 4][i + row] = t4;
          B[j + 5][i + row] = t5;
          B[j + 6][i + row] = t6;
          B[j + 7][i + row] = t7;
        }
      }
    }
  }

  /*
   * The idea behine the 64x64 solution is to take the matrices in 8x8 chunks, then divide
   * those chunks into 4 4x4 quadrants. We use the upper right quadrant of B as a cache.
   * This is efficient since each 8 int line of B shares a cache, just like A does.
   */
  else if(M == 64 && N == 64){
    for(i = 0; i < N; i += 8){
      for(j = 0; j < N; j += 8){
	for(row = 0; row < 4; row++){
	  t0 = A[i + row][j]; //take whole row of A
	  t1 = A[i + row][j + 1];
	  t2 = A[i + row][j + 2];
	  t3 = A[i + row][j + 3];
	  t4 = A[i + row][j + 4];
	  t5 = A[i + row][j + 5];
	  t6 = A[i + row][j + 6];
	  t7 = A[i + row][j + 7];

	  B[j][i + row] = t0; //put first four of A in correct place in B, in upper left quadrant
	  B[j + 1][i + row] = t1;
	  B[j + 2][i + row] = t2;
	  B[j + 3][i + row] = t3;
	  
	  B[j][i + row + 4] = t4; //second four, transpose and store in upper right quadrant of B
	  B[j + 1][i + row + 4] = t5;
	  B[j + 2][i + row + 4] = t6;
	  B[j + 3][i + row + 4] = t7;
	}
	//we now have upper left quadrant of B correct, and upper right holds data for lower left
	for(col = 0; col < 4; col++){
	  t4 = A[i + 4][j + col]; //load lower left quadrant of A
	  t5 = A[i + 5][j + col];
	  t6 = A[i + 6][j + col];
	  t7 = A[i + 7][j + col];

	  t0 = B[j + col][i + 4]; //load upper right quadrant of B
	  t1 = B[j + col][i + 5];
	  t2 = B[j + col][i + 6];
	  t3 = B[j + col][i + 7];

	  B[j + col][i + 4] = t4; //overwrite upper right quadrant of B
	  B[j + col][i + 5] = t5;
	  B[j + col][i + 6] = t6;
	  B[j + col][i + 7] = t7;

	  B[j + col + 4][i] = t0; //place previous upper right data in lower left of B
	  B[j + col + 4][i + 1] = t1;
	  B[j + col + 4][i + 2] = t2;
	  B[j + col + 4][i + 3] = t3;

	  B[j + col + 4][i + 4] = A[i + 4][j + col + 4]; //transfer lower right quadrants
	  B[j + col + 4][i + 5] = A[i + 5][j + col + 4];
	  B[j + col + 4][i + 6] = A[i + 6][j + col + 4];
	  B[j + col + 4][i + 7] = A[i + 7][j + col + 4];
	}
      }
    }
  }
	



  //just use temp variable for diagonals in 16x16 chunks
  else if(M == 61 && N == 67){
    for(i = 0; i < N; i += 16){
      for(j = 0; j < M; j += 16){
	for(row = 0; row < 16 && (row + i) < N; row++){
	  for(col = 0; col < 16 && (col + j) < M; col++){
	    if((i + row) == (j + col)){ //only happens in chunks where i == j
	      t0 = A[i + row][j + col];
	    }
	    else{
	      B[j + col][i + row] = A[i + row][j + col];
	    }
	  }
	  if(i == j){
	    B[i + row][i + row] = t0;
	  }
	}
      }
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

