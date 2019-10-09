#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>
#include <math.h>
#include <windows.h>
#define LIMIT 10000000

unsigned long int out[LIMIT]={0},out2[LIMIT]={0};
unsigned long int prime_count = 0;

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int is_prime(unsigned long int x) {
    for (unsigned long int i=3; i*i <= x; i += 2) {
        if (x%i == 0) return 0;
    }
    return 1;
}

int is_prime_array(unsigned long int x) {
    unsigned long int i=0;
    while ((i<prime_count)&&(out2[i]*out2[i]<=x)) {
        if (out2[i] == 0) return 0;
        if (x % out2[i] == 0) return 0;
        i++;
    }
    return 1;
}

void print_prime (unsigned long int i, unsigned long int *out) {
    printf("%lu ",out[i]);
}

void print_primes(unsigned long int out[]) {
    for (int i=0; i<prime_count; i++)
        printf("%lu ",out[i]);
    printf("\n");
}

void find_primes_brute () {
    if (LIMIT >= 2) {
        out[0] = 2;
        prime_count++;
    }

    //#pragma omp parallel
    {
        //pragma omp for schedule (runtime)
            for (unsigned long int i=3; i<LIMIT; i += 2) {
                if (is_prime(i)) {
                    out[prime_count] = i;
                    prime_count++;
                }
            }
    }
}

void find_primes_array () {
    if (LIMIT >= 2) {
        out2[0] = 2;
        prime_count++;
    }

    //#pragma omp parallel
    {
        //#pragma omp for schedule (runtime)
            for (unsigned long int i=3; i<LIMIT; i += 2) {
                if (is_prime_array(i)) {
                    out2[prime_count] = i;
                    prime_count++;
                }
            }
    }
}

int main() {
    double startCalc, endCalc;
    double runTime;    
    //unsigned long int *out;
    //out = (unsigned long int *) malloc ( ceil(LIMIT/2) * (sizeof(unsigned long int)));


    printf("Calculating all prime numbers under %lu.\n",LIMIT);

    startCalc = omp_get_wtime();
    find_primes_brute(); //10000000 in 0.390000
    endCalc = omp_get_wtime();
    runTime = endCalc - startCalc;
    printf("Calculated all %lu prime numbers under %lu in %f seconds with brute force.\n\n",prime_count, LIMIT, runTime);


    prime_count = 0;

    startCalc = omp_get_wtime();
    find_primes_array();  //10000000 in
    endCalc = omp_get_wtime();
    runTime = endCalc - startCalc;
    printf("Calculated all %lu prime numbers under %lu in %f seconds with array.\n\n",prime_count, LIMIT, runTime);

    //print_primes(out);
    //qsort(out, prime_count, sizeof(out[0]), cmpfunc);
    //print_primes(out2);



    /*
    FILE *f = fopen("primes.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    for (unsigned long i=0; i<prime_count; i++){
        if (out[i]!=0) fprintf(f, "%lu ", out[i]);
    }

    fclose(f);
    */

    return 0;
}
