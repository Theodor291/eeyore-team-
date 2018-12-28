#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

double *cholesky(double *A, int n) {
    double *L = (double*)calloc(n * n, sizeof(double));
    if (L == NULL)
        exit(EXIT_FAILURE);
    
    for (int j = 0; j <n; j++) {
        
        double s = 0;
        for (int k = 0; k < j; k++) {
            s += L[j * n + k] * L[j * n + k];
        }
        L[j * n + j] = sqrt(A[j * n + j] - s);
        for (int i = j+1; i <n; i++) {
            double s = 0;
            for (int k = 0; k < j; k++) {
                s += L[i * n + k] * L[j * n + k];
            }
            L[i * n + j] = (1.0 / L[j * n + j] * (A[i * n + j] - s));
        }
    }
    return L;
}


double *cholesky2(double *A, int n) {
    double *L = (double*)calloc(n * n, sizeof(double));
    if (L == NULL)
        exit(EXIT_FAILURE);
    
    for (int j = 0; j <n; j++) {
        
        double s = 0;
        for (int k = 0; k < j; k++) {
            s += L[j * n + k] * L[j * n + k];
        }
        L[j * n + j] = sqrt(A[j * n + j] - s);
        #pragma omp parallel for
        for (int i = j+1; i <n; i++) {
            double s = 0;
            for (int k = 0; k < j; k++) {
                s += L[i * n + k] * L[j * n + k];
            }
            L[i * n + j] = (1.0 / L[j * n + j] * (A[i * n + j] - s));
        }
    }
    return L;
}



void show_matrix(double *A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%2.5f ", A[i * n + j]);
        printf("\n");
    }
}
 
int main(int argc, char* argv[]) {

    FILE *file;
    int i, j, n;

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
    double *matrix = (double*)malloc(sizeof(double)*n*n);
    double *mat_copy = (double*)malloc(sizeof(double)*n*n);
    if(matrix == NULL)
        exit(EXIT_FAILURE);

    /* read matrix from file */
    for (i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            double aux;
            fscanf(file, "%lf", &aux);
            matrix[i*n+j] = aux;
            mat_copy[i*n+j] = aux;
        }
    }

    /* close file */
    fclose(file);
    
    double *col2 = cholesky2(matrix, n);

    free(matrix);
    free(mat_copy);
    free(col2);
  
    return 0;
    
}
