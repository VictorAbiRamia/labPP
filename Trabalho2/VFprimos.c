#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int primo(long int n) {
    long int i;
    for (i = 3; i < (long int)(sqrt(n) + 1); i += 2) {
        if (n % i == 0) {
            return 0;  // Não é um número primo
        }
    }
    return 1;  // É um número primo
}

int main(int argc, char *argv[]) {
    FILE *fp;
    double t_inicio, t_fim;
    long int n, total = 0;
    long int numberOfthreads[] = {1, 2, 3, 4, 5, 6, 7, 8};
    long int modelsize[] = {1000000, 10000000, 100000000};
    double timematrix[3][7][5];

    // Open file for writing results
    fp = fopen("results.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    for (long int m = 0; m < 3; m++) {
        n = modelsize[m];
        for (long int j = 7; j < 8; j++) {
            for (long int k = 0; k < 5; k++) {
                total = 0;  // Reinicia o total para cada número de threads

                t_inicio = omp_get_wtime();
                //#pragma omp parallel for reduction(+:total) schedule(dynamic, 1000) num_threads(numberOfthreads[j])
                #pragma omp parallel for reduction(+:total) schedule(static) num_threads(numberOfthreads[j])
                for (long int i = 3; i <= n; i += 2) {
                    if (primo(i) == 1) {
                        total++;
                    }
                }

                total += 1;  // Acrescenta o número 2, que também é primo

                t_fim = omp_get_wtime();

                #pragma omp single
                {
                    printf( "Quant. Threads: %ld \n", numberOfthreads[j]);
                    printf( "Quant. de primos entre 1 e %ld: %ld \n", n, total);
                    printf( "Tempo de execução: %3.10f segundos\n", t_fim - t_inicio);
                    timematrix[m][j][k] = t_fim - t_inicio;
                }
            }
        }
    }

    fprintf(fp, "####################\n");
    fprintf(fp, "####################\n");
    fprintf(fp, "###### Results #####\n");
    fprintf(fp, "####################\n");
    fprintf(fp, "####################\n");

    for (long int m = 0; m < 3; m++) {
        fprintf(fp, "####################\n");
        fprintf(fp, "##### n = 10^%ld #####\n", 6 + m);
        fprintf(fp, "####################\n");

        for (long int j = 0; j < 7; j++) {
            fprintf(fp, " %ld Threads\n", numberOfthreads[j]);
            fprintf(fp, " Rodadas: %3.10f, %3.10f, %3.10f, %3.10f, %3.10f\n",
                   timematrix[m][j][0], timematrix[m][j][1], timematrix[m][j][2],
                   timematrix[m][j][3], timematrix[m][j][4]);

            double sum = 0;
            double sum_deviation = 0;

            for (long int k = 0; k < 5; k++) {
                sum += timematrix[m][j][k];
            }
            sum /= 5;

            for (long int k = 0; k < 5; k++) {
                sum_deviation += (timematrix[m][j][k] - sum) * (timematrix[m][j][k] - sum);
            }

            double std_dev = sqrt(sum_deviation / 5);
            fprintf(fp, " Mean: %3.10f\n", sum);
            fprintf(fp, " Standard Deviation: %3.10f\n", std_dev);
            fprintf(fp, " Limits: %3.10f, %3.10f\n", sum - std_dev, sum + std_dev);
        }
    }

    // Close the file
    fclose(fp);

    printf("Results saved to 'results.txt'.\n");

    return 0;
}
