#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>




double *cholesky(double *A, int n) {
    double *L = (double*)calloc(n * n, sizeof(double));
    if (L == NULL)
        exit(EXIT_FAILURE);
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (i+1); j++) {
            double s = 0;
            for (int k = 0; k < j; k++) {
                s += L[i * n + k] * L[j * n + k];
            }
            L[i * n + j] = (i == j) ? sqrt(A[i * n + i] - s) : (1.0 / L[j * n + j] * (A[i * n + j] - s));
        }
    }
    return L;
}


double inner_sum(double *li, double *lj, int n) {
    double s = 0;
    for (int i = 0; i < n; i++) {
        s += li[i]*lj[i];
    }
    return s;
}


double *cholesky2(double *A, int n) {
    double *L = (double*)calloc(n * n, sizeof(double));
    if (L == NULL)
        exit(EXIT_FAILURE);
    
    for (int j = 0; j <n; j++) {
        #pragma omp parallel for
        for (int i = j; i <n; i++) {
            double s = 0;
            for (int k = 0; k < j; k++) {
                s += L[i * n + k] * L[j * n + k];
            }
            L[i * n + j] = (i == j) ? sqrt(A[i * n + i] - s) : (1.0 / L[j * n + j] * (A[i * n + j] - s));
        }
    }
    return L;
}

double *cholesky3(double *A, int n) {
    double *L = (double*)calloc(n * n, sizeof(double));
    if (L == NULL)
        exit(EXIT_FAILURE);
    #pragma omp parallel for
    for (int j = 0; j <n; j++) {
        for (int i = j; i <n; i++) {
            double s = 0;
            for (int k = 0; k < j; k++) {
                s += L[i * n + k] * L[j * n + k];
            }
            L[i * n + j] = (i == j) ? sqrt(A[i * n + i] - s) : (1.0 / L[j * n + j] * (A[i * n + j] - s));
        }
    }
    return L;
}

double *cholesky4(double *A, int n) {
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
 
int main() {
    
    
    int n = 1000;
    double *matrix = (double*)malloc(sizeof(double)*n*n);
    for(int i=0; i<n; i++) {
        for(int j=i; j<n; j++) {
            double element = 1.0*rand();
            matrix[i*n+j] = element;
            matrix[j*n+i] = element;

        }
    }
    

    double time;
    time = omp_get_wtime();
    double *col = cholesky(matrix, n);
    time = omp_get_wtime() - time;
    printf("time %f\n", time);

    time = omp_get_wtime();
    double *col2 = cholesky4(matrix, n);
    time = omp_get_wtime() - time;
    printf("time %f\n", time);

    free(col);
    free(col2);

    
    
    return 0;
    
}