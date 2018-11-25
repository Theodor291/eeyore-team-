#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define THREADS_NUMBER 2

void display_matrix(double *A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%lf ", A[i * n + j]);
        printf("\n");
    }
}

void *cholesky(void *arguments) {
    
}

int main(int argc, char* argv[]) {
	FILE *file;
	int i, j, n;
    double *mat;
    pthread_t threads[THREADS_NUMBER];

    if (argc < 2) {
        fprintf(stderr, "Name of the file needed!\n");
        exit(EXIT_FAILURE);
    }

    /* open file */
    file = fopen(argv[1],"r");
    if (file == NULL)
    	exit(EXIT_FAILURE);

    /* read matrix dimension from file */
    fscanf(file, "%d", &n);
    
    /* allocate memory for initial matrix */
    mat = (double *)malloc(n * n * sizeof(double));
     
    /* read matrix from file */
    for (i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            fscanf(file, "%lf", &mat[i * n + j]);
        }
    }

    /* close file */
    fclose(file);

    /* display initial matrix */
    display_matrix(mat, n);
    printf("\n");

    /* create threads */
    for(i = 0; i < THREADS_NUMBER; i++) {
    	pthread_create(&threads[i], NULL, cholesky, NULL);
    }

    /* join threads */
    for (i = 0; i < THREADS_NUMBER; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}