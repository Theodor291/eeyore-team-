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
 
void display_matrix(double *A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%lf ", A[i * n + j]);
        printf("\n");
    }
}
 
int main(int argc, char* argv[]) {
    FILE *file;
    int i, j, n;
    double *mat;

    if (argc < 2) {
        fprintf(stderr, "Name of the file needed!\n");
        exit(EXIT_FAILURE);
    }

    /* open file */
    file = fopen(argv[1], "r");
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

    /* compute cholesky */
    double *l = cholesky(mat, n);

    /* display resulting matrix */
    display_matrix(l, n);
    
    /* free memory */
    free(mat);
    free(l);
 
    return 0;
}
