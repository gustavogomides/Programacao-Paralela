#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h"

#define TAM 590                /* dimensão da matriz */
#define SERVIDOR 0            /* ID do servidor (MASTER) */
#define ORIGEM_SERVIDOR 1     /* setando mensagem do servidor para o nó */
#define ORIGEM_NO 2           /* setando mensagem do nó para o servidor */

int main (int argc, char *argv[]) {
	int	numTarefas,            /* número de tarefas */
		tarefaID,              /* ID da tarefa */
		qtdNos,                /* número de nós */
		origem,                /* ID da mensagem da tarefa origem */
		destino,               /* ID da mensagem da tarega destino */
		msgTipo,               /* tipo da mensagem */
		linhas,                /* linhas da matriz A que serão enviadas aos nós */
		averow, extra, carga, /* variáveis auxiliares para a determinação da
					  quantidade de linhas que serão enviadas para cada nó */
		i, j, k, rc = -1;      /* variáveis */

	double	a[TAM][TAM],           /* matriz A */
		b[TAM][TAM],           /* matriz B */
		c[TAM][TAM];           /* matriz C (resultante) */

	double tempoInicioEnvio, tempoInicioRecebido, tempoFimEnvio, tempoFimRecebido, tempoEnvio, tempoRecebido;

	MPI_Status status; // status de mensagem MPI

	MPI_Init(&argc,&argv); // inicialização do ambiente de execução do MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&tarefaID); //utilizado para associar um rank do processo requisitante ao comunicador
	MPI_Comm_size(MPI_COMM_WORLD,&numTarefas); //número de processos que estão sendo usados

	if (numTarefas < 2 ) {
		printf("É necessário pelo menos 2 nós para o MPI. Saindo...\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(-1);
	}
	qtdNos = numTarefas - 1;


	/**************************** Tarefas do SERVIDOR ************************************/
	if (tarefaID == SERVIDOR) {
		printf("Multiplição das Matrizes MPI foi iniciada com %d núcleos.\n", numTarefas);

		srand(time(NULL)); //inicialização do gerador randômico

		for (i=0; i<TAM; i++) {
			for (j=0; j<TAM; j++) {
				a[i][j]= (double)rand()/(double)rand(); // Matriz A
				b[i][j]= (double)rand()/(double)rand();	// Matriz B
			}
		}

		/* Enviando os elementos da matriz para os nós */

		tempoInicioEnvio = MPI_Wtime(); //início da contagem do tempo ENVIO

		averow = TAM/qtdNos;
		extra = TAM%qtdNos;
		carga = 0;
		msgTipo = ORIGEM_SERVIDOR;
		for (destino = 1; destino <= qtdNos; destino++){
			linhas = (destino <= extra) ? averow+1 : averow;
			/*if(linhas == 1)
				printf("Enviando %d linha para o núcleo %d, carga = %d\n", linhas, destino, carga);
			else
				printf("Enviando %d linhas para o núcleo %d, carga = %d\n", linhas, destino, carga);*/

			MPI_Send(&carga, 1, MPI_INT, destino, msgTipo, MPI_COMM_WORLD);
			MPI_Send(&linhas, 1, MPI_INT, destino, msgTipo, MPI_COMM_WORLD);
			MPI_Send(&a[carga][0], linhas*TAM, MPI_DOUBLE, destino, msgTipo, MPI_COMM_WORLD);
			MPI_Send(&b, TAM*TAM, MPI_DOUBLE, destino, msgTipo, MPI_COMM_WORLD);

			/* MPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm

				buf: Endereço do buffer de dados a ser enviado, ou seja, o que está sendo enviado.
				count: Número de elementos a ser enviado (número inteiro e não negativo).
				datatype: Tipo do dado.
				dest: Rank do destinatário.
				tag: Identificador para a mensagem (número inteiro).
				comm: Comunicador
			*/

			carga = carga + linhas;
		}
		tempoFimEnvio = MPI_Wtime(); // fim da contagem do tempo ENVIO

		/* Recebendo resultados dos nós */
                tempoInicioRecebido = MPI_Wtime(); //início da contagem do tempo RECEBIDO

		msgTipo = ORIGEM_NO;

		for (i = 1; i <= qtdNos; i++){
			origem = i;
			MPI_Recv(&carga, 1, MPI_INT, origem, msgTipo, MPI_COMM_WORLD, &status);
			MPI_Recv(&linhas, 1, MPI_INT, origem, msgTipo, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[carga][0], linhas*TAM, MPI_DOUBLE, origem, msgTipo, MPI_COMM_WORLD, &status);


			/* MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
				buf: Endereço do buffer para receber os dados,ou seja, o que está sendo enviado.
				count: Número de elementos a ser recebido (número inteiro e não negativo).
				datatype: Tipo do dado.
				source: Rank do remetente.
				tag: Identificador para a mensagem (número inteiro).
				comm: Comunicador.
				Status: Status da mensagem
			*/

			//printf("Recebendo resultados do núcleo %d\n", origem);
		}
                tempoFimRecebido = MPI_Wtime(); // fim da contagem do tempo RECEBIDO

		tempoEnvio = tempoFimEnvio - tempoInicioEnvio;
                tempoRecebido = tempoFimRecebido - tempoInicioRecebido;

		/* Imprimindo resultados */
	        printf("******************************************************\n");
		printf("\nMatriz A:\n");
                for (i = 0; i < TAM; i++) {
                        for (j=0; j<TAM; j++) {
                                printf("%lf   ", a[i][j]);
                        }
                        printf("\n");
                }
                printf("\n******************************************************\n");

		printf("\nMatriz B:\n");
                for (i = 0; i < TAM; i++) {
                        for (j=0; j<TAM; j++) {
                                printf("%lf   ", b[i][j]);
                        }
                        printf("\n");
                }
                printf("\n******************************************************\n");


	        printf("\nMatriz Resultante:\n");
	        for (i = 0; i < TAM; i++) {
			for (j=0; j<TAM; j++) {
		        	printf("%lf   ", c[i][j]);
			}
			printf("\n");
		}
	        printf("\n******************************************************\n");

                printf("\nTempo de envio dos elementos para os nós = %lf segundos\n", tempoEnvio);
                printf("Tempo de recebimento dos elementos dos nós = %lf segundos\n", tempoRecebido);
		printf("Tempo Total = %lf segundos\n", tempoEnvio + tempoRecebido);

	        printf ("\nFinalizado.\n\n");

		}


	/**************************** Tarefas dos nós ************************************/
	if (tarefaID > SERVIDOR) {
		msgTipo = ORIGEM_SERVIDOR;
		MPI_Recv(&carga, 1, MPI_INT, SERVIDOR, msgTipo, MPI_COMM_WORLD, &status);
		MPI_Recv(&linhas, 1, MPI_INT, SERVIDOR, msgTipo, MPI_COMM_WORLD, &status);
		MPI_Recv(&a, linhas*TAM, MPI_DOUBLE, SERVIDOR, msgTipo, MPI_COMM_WORLD, &status);
		MPI_Recv(&b, TAM*TAM, MPI_DOUBLE, SERVIDOR, msgTipo, MPI_COMM_WORLD, &status);

		for (k = 0; k < TAM; k++) {
			for (i = 0; i < linhas; i++) {
				c[i][k] = 0.0;
				for (j = 0; j < TAM; j++)
					c[i][k] = c[i][k] + a[i][j] * b[j][k];
			}
		}

		msgTipo = ORIGEM_NO;
		MPI_Send(&carga, 1, MPI_INT, SERVIDOR, msgTipo, MPI_COMM_WORLD);
		MPI_Send(&linhas, 1, MPI_INT, SERVIDOR, msgTipo, MPI_COMM_WORLD);
		MPI_Send(&c, linhas*TAM, MPI_DOUBLE, SERVIDOR, msgTipo, MPI_COMM_WORLD);
	}
	MPI_Finalize();

	return 0;
}
