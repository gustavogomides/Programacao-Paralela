#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define QTD_PONTOS 16
#define DELTA_T 0.01

void funcao_base(double ft[]);

int main () {
    int i;
    double ft[QTD_PONTOS];
    
    FILE *fp = fopen("ft.txt", "w");

    if(!fp){
        printf("Erro na criação do arquivo!\n");
        exit(-1);
    }

    funcao_base(ft);

    for(i = 0; i< QTD_PONTOS; i++) 
        fprintf(fp, "%.3lf\n", ft[i]);    

    printf("\nValores de f(t) gerados e armazenados no arquivo ft.txt\n");

    fclose(fp);
    return 0;
}

void funcao_base(double ft[]){
    int i;
    double t = 0;
    double pi = 3.14159265358979323846264338327950288419716939937510;

    for(i = 0; i < QTD_PONTOS; i++){
        ft[i] = cos(2 * pi * (12.5 * t));
        t += DELTA_T;
    }   
}
