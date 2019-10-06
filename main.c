#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#define LIMIT 1000000

int is_prime(unsigned long int x) {
    //if (x <= 1) return 0; // zero and one are not prime
    //if(x != 2 && x % 2 == 0) return 0; //there is no need to check even numbers
    for (unsigned long int i=3; i*i <= x; i += 2) {
        if (x%i == 0) return 0;
    }
    return 1;
}

int main() {
    double startCalc, endCalc;
    double runTime;
    unsigned long int prime_count = 0, limit = LIMIT;
    unsigned long int *out;
    out = (unsigned long int *) malloc ( ceil(limit/2) * (sizeof(unsigned long int)));

    startCalc = omp_get_wtime();

    if (limit >= 2) {
        out[prime_count] = 2;
        prime_count++;
        //out[(int)ceil(limit/2)] = 0;
    }

    #pragma omp parallel
    {
        #pragma omp for schedule (runtime)
        for (unsigned long int i=3; i<=limit; i += 2) {
            if (is_prime(i)) {
                out[prime_count] = i;
                prime_count++;
            }
        }
    }


    endCalc = omp_get_wtime();
    runTime = endCalc - startCalc;

    double startPrint = omp_get_wtime();
    for (int k=0; k<prime_count; k++){
        printf("%lu ",out[k]);
    }
    double endPrint = omp_get_wtime();
    double printTime = endPrint - startPrint;


    printf("\n\nCalculated all %lu prime numbers under %lu in %g seconds\n\n",prime_count, limit, runTime);
    printf("\n\nPrinting all %lu prime numbers under %lu took %g seconds to print\n\n",prime_count, limit, printTime);


    return 0;
}


