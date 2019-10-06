#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>
#include <math.h>
#include <windows.h>
#define LIMIT 100000

int is_prime(unsigned long int x) {
    //if (x <= 1) return 0; // zero and one are not prime
    //if(x != 2 && x % 2 == 0) return 0; //there is no need to check even numbers
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

    startCalc = omp_get_wtime();

    if (limit >= 2) {
        out[0] = 2;
        prime_count++;
        //out[(int)ceil(limit/2)] = 0;
    }

    #pragma omp parallel
    {
        #pragma omp for schedule (runtime)
            for (unsigned long int i=3; i<=limit; i += 2) {
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


/*
 * // This is painfully slow
    int nullCount = 0;
    int newLimit = floor(limit/2);
    for (int i=0; i<newLimit; i++) {
        nullCount=0;
        if (out[i]==0) {
            int j = i;
            while (out[j]==0) {
                nullCount++;
                j++;
            }

            for (int j=i; j<floor(limit/2); j++) {
                out[j]=out[j+nullCount];
                out[j+nullCount]=0;
            }
            newLimit-=nullCount;
        }
    }

    printf("Primes:\t");
    for (int i=0; i<newLimit; i++) {
        printf("%lu\t",out[i]);
    }
*/
    char yesno = 'n';
    printf("Do you want to print the primes up to %lu? (y/n) ", limit);
    scanf("%c", &yesno);
    if ((yesno == 'y') || (yesno == 'Y')) {
        double startPrint = omp_get_wtime();
        for (unsigned long i=0; i<=floor(limit/2); i++){
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


