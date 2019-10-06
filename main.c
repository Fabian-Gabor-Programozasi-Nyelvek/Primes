#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int is_prime(unsigned long int x) {
    unsigned long int half = sqrt(x);

    for (unsigned long int i=3; i <= half; i += 2) {
        if (x%i == 0) return 0;
    }
    return 1;
}

int main() {
    double start, end;
    double runTime;
    unsigned long int prime_count = 2, limit = 100000;
    start = omp_get_wtime();

    printf("1 2 ");
    for (unsigned long int i=3; i<=limit; i += 2) {
        if (is_prime(i)) {
            prime_count++;
            printf("%lu ", i);
        }
    }

    end = omp_get_wtime();
    runTime = end - start;
    printf("\n\nCalculated all %lu prime numbers under %lu in %g seconds\n\n",prime_count, limit, runTime);
    return 0;
}


