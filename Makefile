run_openmp: openmp
	./cholesky_openmp mat.txt

run_pthreads: pthreads
	./cholesky_pthreads mat.txt

run_serial: serial
	./cholesky_serial mat.txt

openmp: cholesky_openmp.c
	gcc cholesky_openmp.c -fopenmp -o cholesky_openmp -lm -std=c11

pthreads: cholesky_pthreads.c
	gcc cholesky_pthreads.c -pthread -o cholesky_pthreads -std=c11

serial: cholesky_serial.c
	gcc cholesky_serial.c -lm -o cholesky_serial -std=c11

clean:
	rm -f cholesky_serial cholesky_pthreads cholesky_openmp
