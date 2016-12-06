#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#define INTERVALOS 1e3

int main(int argc, char *argv[]){
        //Declarações
        int i, nucleos, tid;
	double n = INTERVALOS;
        double pi, h, sum, x;
        double piExato = 3.1415926535897932, diferenca;
	double t1, t2;

	#pragma omp parallel
        {
                tid = omp_get_thread_num();
                if (tid == 0) {
                        nucleos = omp_get_num_threads();
                        printf("\nCálculo do PI OpenMP foi iniciado com %d núcleos e %.0lf intervalos\n", nucleos, n);
                }
        }

	t1 = omp_get_wtime();

	h = 1.0/ (double)n;
	sum = 0.0;
	#pragma omp parallel for private(x) reduction(+:sum)
		for(i = n; i >= 1; i--){
                        x = h * ((double)i - 0.5);
                        sum += (4.0/(1.0 + x * x));
                }
	pi = h * sum;

	t2 = omp_get_wtime();

        diferenca = fabs(pi - piExato);
        printf("\n\nValor exato do PI: %.16lf\n", piExato);
        printf("Valor aproximado pi com %.0lf intervalos: %.16lf\n", n, pi);
        printf("Diferença entre o valor exato e o valor encontrado: %.16lf\n", diferenca);
	printf("Tempo de cálculo: %lf ms\n\n", t2 - t1);
        return 0;
}


