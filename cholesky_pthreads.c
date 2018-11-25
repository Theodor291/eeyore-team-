#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

#define THREADS_NUMBER 2

void display_matrix(double *A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%lf ", A[i * n + j]);
        printf("\n");
    }
}

void *cholesky(void *param) {
    
}

double * multiply_matrix_with_transpose(double *l, int n) {
    int i, j, k;
    double *result = (double *)calloc(n * n, sizeof(double));

    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            for(k = 0; k < n; k++)
                result[i * n + j] += l[i * n + k] * l[j * n + k];

    return result;
}


bool check_for_correctness(double *a, double *l, int n, double precision) {
    int i, j, k;
    bool ok = true;
    double *result = multiply_matrix_with_transpose(l, n);

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(abs(a[i*n+j] - result[i*n+j]) > precision){
                ok = false;
                break;
            }
        }
    }

    free(result);

    return ok;
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
    if(mat == NULL)
        exit(EXIT_FAILURE);
     
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

    display_matrix(mat, n);

    return 0;
}