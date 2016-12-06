#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define INTERVALOS 2e10

int main(int argc, char *argv[]){
	//Declarações
	double n = INTERVALOS;
	int myrank, p, i, rc = -1;
	double mypi, pi, h, x, sum = 0.0;
	double tempoInicio, tempoFim;
	double piExato = 3.14159265358979323846264338327950288419716939937510;
	double diferenca;

	MPI_Init(&argc,&argv); // inicialização do ambiente de execução do MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank); //utilizado para associar um rank do processo requisitante ao comunicador
	MPI_Comm_size(MPI_COMM_WORLD, &p); //número de processos que estão sendo usados

	MPI_Bcast(&n, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (p < 2 ) {
		printf("\n\n|******************************************************|\n");
                printf("|                                                      |\n");
                printf("|  É necessário pelo menos 2 nós para o MPI. Saindo... |\n");
                printf("|                                                      |\n");
                printf("|******************************************************|\n\n");

		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(-1);
	}
	if(myrank == 0){
		printf("\nCálculo do pi MPI foi iniciada com %d núcleos e %.0lf intervalos\n", p, n);
	}
	if(n != 0){
		tempoInicio = MPI_Wtime();
		h = 1.0/n;
		for(i=myrank+1; i <= n; i+=p){
			x = h * ((double)i - 0.5);
			sum += (4.0/(1.0 + x * x));
		}
		mypi = h * sum;
		MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

		/*
			Combina os dados de todos os processos do comunicador usando alguma operação em uma única variável, e returna o resultado para o servidor
			MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm); 
				sendbuf: endereço do elemento que será enviado ao buffer, ou seja, a variável que será reduzida (mypi) 
				count: número de elementos que será enviado ao buffer (1)
				datatype: tipo dos dados que serão enviadas ao buffer (MPI_DOUBLE)
				op: operação a ser realizada (MPI_SUM)
				root: rank do servidor (0)
				comm: comunicador (MPI_COMM_WORLD)
		*/

		tempoFim = MPI_Wtime();
		if(myrank == 0){
			diferenca = fabs(pi - piExato);
			printf("\n\nValor exato do PI: %.50lf\n", piExato);
			printf("Valor aproximado pi com %.0lf intervalos: %.50lf\n", n, pi);
			printf("Diferença entre o valor exato e o valor encontrado: %.50lf\n", diferenca);
			printf("Tempo de cálculo: %lf ms\n\n", (tempoFim - tempoInicio)*1000);
		}

	} 

	MPI_Finalize();

	return 0;
}


