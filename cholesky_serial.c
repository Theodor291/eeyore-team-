#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
double *cholesky(double *A, int n) {
    double *L = (double*)calloc(n * n, sizeof(double));
    if (L == NULL)
        exit(EXIT_FAILURE);
 
    for (int i = 0; i < n; i++)
        for (int j = 0; j < (i+1); j++) {
            double s = 0;
            for (int k = 0; k < j; k++)
                s += L[i * n + k] * L[j * n + k];
            L[i * n + j] = (i == j) ?
                           sqrt(A[i * n + i] - s) :
                           (1.0 / L[j * n + j] * (A[i * n + j] - s));
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
    int n = 3;
    FILE *arr;
    double m1[n][n];
    arr =fopen("mat.txt","r");
    int i,j;
     
     
    for(i=0 ;i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            fscanf(arr, "%lf",&m1[i][j]);
            printf("%f\n", m1[i][j]);
        }
    }
    double *c1 = cholesky(m1, n);
    show_matrix(c1, n);
    printf("\n");
    free(c1);
 
 
    return 0;
}
