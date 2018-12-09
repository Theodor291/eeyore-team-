#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int * multiply_matrix_with_transpose(int *l, int n) {
    int i, j, k;
    int *result = (int *)calloc(n * n, sizeof(int));

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            int sum = 0;
            for(k = 0; k < n; k++){
                sum += l[i * n + k] * l[j * n + k];
            }
            result[i * n + j] = sum;
        }
    }

    return result;
}

void display_matrix(int *A, int n) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++){
            printf("%d ", A[i * n + j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
	int i, j, n;
	int *l;
	int *mat;
	FILE *file;

	if (argc < 3) {
        fprintf(stderr, "Provide matrix dimension and name of output file!\n");
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    /* allocate matrix of given dimension */
    l = (int *)calloc(n * n, sizeof(int));
    if(l == NULL)
    	exit(EXIT_FAILURE);

    srand(time(0));

    /* populate lower triangular matrix */
    for(i = 0; i < n; i++) {
    	for(j = 0; j < i; j++) {
    		l[i * n + j] = rand() % 20;
    	}
    	l[i * n + i] = rand() % 20;
    }
    //printf("L\n");
    // display_matrix(l, n);
    // printf("\n");

    mat = multiply_matrix_with_transpose(l, n);

    //display_matrix(mat, n);

    file = fopen(argv[2], "w");
	if (file == NULL) {
    	printf("Error opening file!\n");
    	exit(1);
	}

	fprintf(file, "%d\n", n);

	for(i = 0; i < n; i++) {
    	for(j = 0; j < n; j++) {
    		fprintf(file, "%d ", mat[i * n + j]);
    	}
    	fprintf(file, "\n");
	}


	fclose(file);

	

	return 0;
}