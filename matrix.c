#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX 4

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 
int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

struct test{
  size_t i, j;
};
void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = (rand() % 10 + 1);
        }
    }
}
void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void* computeSum(void* args) {// Fetches the appropriate coordinates from the argument
  int coordinate = *(int*)args;
  for (int i = coordinate * MAX / 4; i < (coordinate + 1) * MAX / 4; i++){// pass in the number of the ith thread
    for (int j = 0; j < MAX; j++){
      matSumResult[i][j] = matA[i][j] + matB[i][j];// and sets the cell of matSumResult at the coordinates to the sum of the
    }// values at the coordinates of matA and matB.
  }
    return NULL;
}

void* computeDiff(void* args) {// Fetches the appropriate coordinates from the argument
  int coordinate = *(int*)args;
  for (int i = coordinate * MAX / 4; i < (coordinate + 1) * MAX / 4; i++){
    for (int j = 0; j < MAX; j++){
      matDiffResult[i][j] = matA[i][j] - matB[i][j];//and sets the cell of matSumResult at the coordinates to the difference of the
    }// values at the coordinates of matA and matB.
  }
    return NULL;
}

void* computeProduct(void* args) {// Fetches the appropriate coordinates from the argument, and sets
  struct test *data = (struct test *)args;
  size_t n,m;
  for (n = 0; n < MAX; n++){
    size_t i = (data[n]).i;
    size_t j = (data[n]).j;
    double sum = 0;
    for (m = 0; m < MAX; m++){
      sum = sum + matA[i][m]*matB[m][j];//to the inner product of matA and matB.
    }
    matProductResult[i][j] = sum;// and sets the cell of matSumResult at the coordinates
    sum = 0;
      
  }
    
    return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?

int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    
    // 1. Fill the matrices (matA and matB) with random values.
    
    fillMatrix(matA);
      fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
      printMatrix(matA);
      printf("Matrix B:\n");
      printMatrix(matB);
     
    pthread_t threads[MAX];// 3. Create pthread_t objects for our threads.
    
    // 4. Create a thread for each cell of each matrix operation.
    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute. One way to do this is to "encode" the coordinates into a number
    // and then pass it into the thread function. For example,
    // 
    // long coordinates = MAX*row + col;
    // 
    // Then the thread will have to "decode" that number back into the row and col.
    // 
    // Another option is to malloc memory for the coordinates, populate the coordinates
    // into that space, and pass that address to the thread. The thread will then have
    // to free that space when it's done with what's in that memory.
    
    size_t x;
    size_t y;
    struct test **numbers;
    numbers = ((struct test **)malloc(MAX * sizeof (struct test)));
    for (x = 0; x < MAX; x++){
      numbers[x] = (struct test *)malloc(MAX * sizeof (struct test));
      for (y = 0; y < MAX; y++){
        numbers[x][y].i = x;
        numbers[x][y].j = y;
      }
      pthread_create(&threads[x], NULL, computeProduct, numbers[x]);
        pthread_create(&threads[x], NULL, computeSum, numbers[x]);
        pthread_create(&threads[x + MAX], NULL, computeDiff, numbers[x]);
    }free(numbers);
    
  for (x = 0; x < MAX; x++){// 5. Wait for all threads to finish.
    pthread_join(threads[x], NULL);
  }
    // 6. Print the results.
    printf("Results:\n");
      printf("Sum:\n");
      printMatrix(matSumResult);
      printf("Difference:\n");
      printMatrix(matDiffResult);
      printf("Product:\n");
      printMatrix(matProductResult);
    return 0;
}
