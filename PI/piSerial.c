#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define INTERVALOS 1e9

int main(int argc, char *argv[]){
        //Declarações
        int i;
	double n = INTERVALOS;
        double pi, h, sum, x, tempo;
        double piExato = 3.1415926535897932, diferenca;

	clock_t start, end;

	start = clock();

        h = 1.0/n;
        sum = 0.0;
        for(i = 1; i <= n; i++){
                x = h * ((double)i - 0.5);
                sum += (4.0/(1.0 + x * x));
        }
        pi = h * sum;

	end = clock();
	tempo = 1000*(double)(end-start)/(double)(CLOCKS_PER_SEC);

        diferenca = fabs(pi - piExato);
        printf("Valor exato do PI: %.16lf\n", piExato);
        printf("Valor aproximado pi com %.0lf intervalos: %.16lf\n", n, pi);
        printf("Diferença entre o valor exato e o valor encontrado: %.16lf\n", diferenca);
	printf("Tempo de cálculo: %lf ms\n\n", tempo);
        return 0;
}

