#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define QTD_PONTOS 16

int main() {
    int k, n, i;
    double angulo, w0;
    double pi = 3.14159265358979323846264338327950288419716939937510;
    double real[QTD_PONTOS], imaginario[QTD_PONTOS], f[QTD_PONTOS];

    FILE *fp = fopen("ft.txt", "r");

    if(!fp){
        printf("Erro na leitura do arquivo!\n");
        exit(-1);
    }

    w0 = ((2 * pi) / QTD_PONTOS);

    for (i = 0; i < QTD_PONTOS; i++)
        fscanf(fp, "%lf", &f[i]);

    printf("\nÍNDICE\t\tf(t)\t\t REAL\t\tIMAGINARIO\n");
    for (k = 0; k < QTD_PONTOS; k++) {
        for (n = 0; n < QTD_PONTOS; n++) {
            angulo = k * w0 * n;
            real[k] = real[k] + ((f[n] * cos(angulo)) / QTD_PONTOS);
            imaginario[k] = imaginario[k] - ((f[n] * sin(angulo)) / QTD_PONTOS);

        }
    }


    for (i = 0; i < QTD_PONTOS; i++) {
        if(real[i] < 0)
            real[i] *= -1;
        if(imaginario[i] < 0)
            imaginario[i] *= -1;
        printf("%d\t\t%.3lf\t\t%.3lf\t\t%.3lf\n", i, f[i], real[i], imaginario[i]);
    }

    fclose(fp);
    return 0;
}


/*

1.000
0.707
0.000
-0.707
-1.000
-0.707
0.000
0.707
1.000
0.707
0.000
-0.707
-1.000
-0.707
0.000
0.707

 */





