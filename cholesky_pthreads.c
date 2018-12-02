#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define THREADS_NUMBER 5

pthread_barrier_t barrier;

typedef struct my_struct {
    int tid;
    double *mat;
    int n;
} my_struct;

void display_matrix(double *A, int n) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++){
            printf("%lf ", A[i * n + j]);
        }
        printf("\n");
    }
}

void *cholesky(void *param) {
    int i, j, k;
    my_struct *aux = param;
    int tid = aux->tid;
    double *mat = aux->mat;
    int n = aux->n;

    for(j = 0; j < n; j++) {
        pthread_barrier_wait(&barrier);
        
        double s = mat[j * n + j];

        if (tid==0) {
            for (k = 0; k < j; k++) {
                s -= mat[j * n + k] * mat[j * n + k];
            }
            mat[j * n + j] = sqrt(s);
        }

    
        pthread_barrier_wait(&barrier);

        for (i = tid; i < n; i+=THREADS_NUMBER) {
            if (i > j) {
                s = mat[j*n+i];
                for (k = 0; k < j; k++) {
                    s -= mat[j * n + k] * mat[k * n + i];
                }
               
                mat[j * n + i] = s/mat[j * n + j];
                mat[i*n + j] = mat[j*n+i];
            }
            else if(j == n-1){
                int k;
                for(k = j; k > i; k--){
                    mat[k*n+i] = 0;
                }
            }
        
        }
    }

}

double * multiply_matrix_with_transpose(double *l, int n) {
    int i, j, k;
    double *result = (double *)calloc(n * n, sizeof(double));

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            double sum = 0;
            for(k = 0; k < n; k++){
                sum += l[k * n + i] * l[k * n + j];
            }
            result[i * n + j] = sum;
        }
    }

    // printf("da\n");
    // display_matrix(result, n);
    // printf("\n");

    return result;
}

bool compare_matrix_with_precision(double *a, double *l, int n, double precision) {
    int i, j;
    bool ok = true;

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(abs(a[i * n + j] - l[i * n + j]) > precision){
                printf("%lf %lf\n", a[i * n + j], l[i * n + j]);
                ok = false;
                break;
            }
        }
    }

    return ok;
}

void check_for_correctness(double *a, double *l, int n) {
    double *result = multiply_matrix_with_transpose(l, n);
   
    if(compare_matrix_with_precision(a, result, n, 0))
        printf("Correct!\n");
    else
        printf("Wrong!\n");

    free(result);
}



int main(int argc, char* argv[]) {
    FILE *file;
    int i, j, n;
    double *mat, *mat_copy;

    pthread_t threads[THREADS_NUMBER];
    my_struct structs[THREADS_NUMBER];


    int ret = pthread_barrier_init(&barrier, NULL, THREADS_NUMBER);
    if(ret)
        printf("Error barrier!!\n");


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
    mat_copy = (double *)malloc(n * n * sizeof(double));
    if(mat == NULL || mat_copy == NULL)
        exit(EXIT_FAILURE);

    /* read matrix from file */
    for (i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            double aux;
            fscanf(file, "%lf", &aux);
            mat_copy[i*n+j] = aux;
            mat[i*n+j] = aux;
        }
    }

    /* close file */
    fclose(file);

    for(i = 0; i < THREADS_NUMBER; i++) {
        structs[i].tid = i;
        structs[i].n = n;
        structs[i].mat = mat;
    }

    /* create threads */
    for(i = 0; i < THREADS_NUMBER; i++) {
        pthread_create(&threads[i], NULL, cholesky, (void *)&structs[i]);
    }

    /* join threads */
    for (i = 0; i < THREADS_NUMBER; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&barrier);
    printf("Result!\n");
    display_matrix(mat, n);

    // for(i = 0; i < n; i++) {
    //     for(j = 0;j <= i; j++){
    //         mat[i * n + j] = mat[j * n + i];
    //         if(i!=j){
    //             mat[j*n+i] = 0;
    //         }
    //     }
    // }

    // printf("Results!\n");
    // display_matrix(mat, n);

    check_for_correctness(mat_copy, mat, n);

    /* free memory */
    free(mat);
    free(mat_copy);
    
    return 0;
}