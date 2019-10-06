#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>
#include <math.h>
#include <windows.h>
#define LIMIT 1000000

int is_prime(unsigned long int x) {
    for (unsigned long int i=3; i*i <= x; i += 2) {
        if (x%i == 0) return 0;
    }
    return 1;
}

void print_prime (unsigned long int i, unsigned long int *out) {
    printf("%lu ",out[i]);
}

int main() {
    double startCalc, endCalc;
    double runTime;
    unsigned long int prime_count = 0, limit = LIMIT;
    unsigned long int *out;
    out = (unsigned long int *) malloc ( ceil(limit/2) * (sizeof(unsigned long int)));

    printf("Calculating all prime numbers under %lu.\n",limit);

    startCalc = omp_get_wtime();

    if (limit >= 2) {
        out[0] = 2;
        prime_count++;
    }

    #pragma omp parallel
    {
        #pragma omp for schedule (runtime)
            for (unsigned long int i=3; i<limit; i += 2) {
                if (is_prime(i)) {
                    out[(int)floor(i/2)] = i;
                    #pragma omp critical
                        prime_count++;
                }
                else {
                    out[(int)floor(i/2)]=0;
                }
            }
    }
    endCalc = omp_get_wtime();
    runTime = endCalc - startCalc;

    printf("Calculated all %lu prime numbers under %lu in %g seconds\n\n",prime_count, limit, runTime);

    char yesno = 'n';
    printf("Do you want to print the primes up to %lu? (y/n) ", limit);
    scanf("%c", &yesno);
    if ((yesno == 'y') || (yesno == 'Y')) {
        double startPrint = omp_get_wtime();
        for (unsigned long i=0; i<floor(limit/2); i++){
            if (out[i]!=0) printf("%lu ",out[i]);
        }

        double endPrint = omp_get_wtime();
        double printTime = endPrint - startPrint;

        printf("\n\nCalculated all %lu prime numbers under %lu in %g seconds\n",prime_count, limit, runTime);
        printf("Printing all %lu prime numbers under %lu took %g seconds to print\n\n\n",prime_count, limit, printTime);
    }

    printf("\n");

    return 0;
}
