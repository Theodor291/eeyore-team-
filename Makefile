run_openmp: openmp
	./cholesky_openmp mat.txt

run_pthreads: pthreads
	./cholesky_pthreads mat.txt

run_serial: serial
	./cholesky_serial mat.txt

openmp: cholesky_openmp.
	gcc cholesky_openmp.c -fopenmp -o cholesky_pthreads

pthreads: cholesky_pthreads.c
	gcc cholesky_pthreads.c -pthread -lm -o cholesky_pthreads

serial: cholesky_serial.c
	gcc cholesky_serial.c -lm -o cholesky_serial

clean:
	rm -f cholesky_serial cholesky_pthreads cholesky_openmp