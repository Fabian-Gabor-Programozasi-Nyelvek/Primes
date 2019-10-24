#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>
//#include <pthread.h>
#include <math.h>
#include <windows.h>
#define LIMIT 100000000
//#define LIMIT 1000*1000*100
//#define OMP_NUM_OF_THREADS omp_get_max_threads()
#define OMP_NUM_OF_THREADS 8

unsigned long long int out[LIMIT]={0},out2[LIMIT]={0};
unsigned long long int prime_count = 0;

typedef struct {
    unsigned long long int info;
} DATA;

typedef struct node {
    DATA data;
    struct node* next;
} NODE;

NODE* head;
NODE* node;
DATA element;

void init(NODE** head) {
    *head = NULL;
}

NODE* add(NODE* node, DATA data) {
    NODE* temp = (NODE*) malloc(sizeof (NODE));
    if (temp == NULL) {
        exit(0); // no memory available
    }
    temp->data = data;
    temp->next = node;
    node = temp;
    return node;
}

NODE *getLastItem_rec(NODE *head)
{
    if (head == NULL) // return NULL if list is empty
        return NULL;
    if (head -> next == NULL) // return 1st element if it's a single element list
        return head;
    return getLastItem_rec(head->next); // recurse
}

NODE *getLastItem(NODE *head)
{
    NODE *currentElement;

    currentElement = head;
    if (currentElement == NULL) // return NULL is list is empty cf (Captain girafe && learningC)
        return (NULL);
    while(currentElement->next != NULL) // check if next element is null then currentElement = next else return currentElement
      currentElement = currentElement->next;
    return (currentElement);
}

void print_list(NODE* head) {
    NODE * temp;
    for (temp = head; temp; temp = temp->next)
        printf("%d ", temp->data.info);
    printf("\n");
}

NODE *bubble_sort_list(NODE *head) {
    NODE *tmpPtr = head, *tmpNxt = head->next;
    DATA tmp;
    while (tmpNxt != NULL) {
        while (tmpNxt != tmpPtr) {
            if (tmpNxt->data.info < tmpPtr->data.info) {
                tmp = tmpPtr->data;
                tmpPtr->data = tmpNxt->data;
                tmpNxt->data = tmp;
            }
            tmpPtr = tmpPtr->next;
        }
        tmpPtr = head;
        tmpNxt = tmpNxt->next;
    }
    return tmpPtr;
}

NODE *get_max_list(NODE *head) {
    NODE * temp;
    unsigned long long int max=2;
    for (temp = head; temp; temp = temp->next)
        if (temp->data.info > max)
            max = temp->data.info;
    return max;
}


int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int is_prime(unsigned long int x) {
    for (unsigned long int i=3; i*i <= x; i += 2) {
        if (x%i == 0) return 0;
    }
    return 1;
}

int is_prime_array (unsigned long int x) {
    unsigned long int i=0;
    while ((i<prime_count)&&(out2[i]*out2[i]<=x)) {
        if (out2[i] == 0) return 0;
        if (x % out2[i] == 0) return 0;
        i++;
    }    
    return 1;
}

int is_prime_array_parallel (unsigned long int x) {
    boolean unprime = false;
    #pragma omp parallel default(none) shared(out2, unprime, prime_count, x)
    {
        #pragma omp for
            for (unsigned long int i=0; i<prime_count; i++) {
                if (x % out2[i] == 0) {
                    unprime = true;
                    #pragma omp cancel for
                }
                #pragma omp cancellation point for
            }
    }

    if (unprime) return 0;
    return 1;
}



void print_prime (unsigned long int i, unsigned long int *out) {
    printf("%lu ",out[i]);
}

void print_primes(unsigned long long int out[]) {
    for (unsigned long long int i=0; i<prime_count; i++)
        printf("%llu ",out[i]);
    printf("\n");
}

void find_primes_brute () {
    if (LIMIT >= 2) {
        out[0] = 2;
        prime_count++;
    }
    for (unsigned long long int i=3; i<LIMIT; i += 2) {
        if (is_prime(i)) {
            out[prime_count] = i;
            prime_count++;
        }
    }

}

void find_primes_array () {
    if (LIMIT >= 2) {
        out2[0] = 2;
        prime_count++;
    }

    for (unsigned long long int i=3; i<LIMIT; i += 2) {
        if (is_prime_array(i)) {
            out2[prime_count] = i;
            prime_count++;
        }
    }
}

void find_primes_array_parallel () {
    if (LIMIT >= 2) {
        out2[0] = 2;
        prime_count++;
    }

    //#pragma omp parallel
    {
        //#pragma omp for schedule (runtime)
            for (unsigned long long int i=3; i<LIMIT; i += 2) {
                if (is_prime_array_parallel(i)) {
                    out2[prime_count] = i;
                    prime_count++;
                }
            }
    }
}

void find_primes_threaded () {

    init(&head);
    element.info = 2;
    head = add(head, element);

    unsigned long long global_per_thread_sub_count[OMP_NUM_OF_THREADS];
    #pragma omp parallel num_threads(      OMP_NUM_OF_THREADS )
    {
        int        thread_local_ID      = omp_get_thread_num();
        int        thread_local_COUNT   =  0;

        //printf( "INF(t:%3d)|||: _id\n",                       thread_local_ID );

        for ( unsigned long long int i  = ( 3 + ( 2 * thread_local_ID ) );
                         i <  LIMIT;
                         i += ( 2 * OMP_NUM_OF_THREADS )
                         )
        {

            if ( is_prime(i) ) {
                //out3[thread_local_COUNT + thread_local_ID * OMP_NUM_OF_THREADS ] = i;
                element.info = i;
                head = add(head, element);
                thread_local_COUNT++;
                //printf("out3[ %d + %d * %d ] = %lli \n", thread_local_COUNT, thread_local_ID, OMP_NUM_OF_THREADS, i);
            }
        }
        global_per_thread_sub_count[thread_local_ID] = thread_local_COUNT;

    }
    prime_count = 1;

    for ( int thread_OrdNo  = 0;
              thread_OrdNo <  OMP_NUM_OF_THREADS;
              thread_OrdNo++
              )
    {   prime_count+= global_per_thread_sub_count[thread_OrdNo];
        /*
        printf( ":--(t:%3d):--:%12d\t%12d\n",(int)thread_OrdNo,
                 (int)global_per_thread_bracket[  thread_OrdNo],
                 (int)global_per_thread_sub_count[thread_OrdNo]
                 );
        */
    }
}

int main() {
    double startCalc, endCalc;
    double runTime;    
    //unsigned long int *out;
    //out = (unsigned long int *) malloc ( ceil(LIMIT/2) * (sizeof(unsigned long int)));


    printf("Calculating all prime numbers under %llu.\n",LIMIT);
    /*
    startCalc = omp_get_wtime();
    find_primes_brute(); // 10000000 in 1.631000
    endCalc = omp_get_wtime();
    runTime = endCalc - startCalc;
    printf("Calculated all %lu prime numbers under %lu in %f seconds with brute force.\n",prime_count, LIMIT, runTime);

    prime_count = 0;
    startCalc = omp_get_wtime();
    find_primes_array();  // 10000000 in 0.828000
    endCalc = omp_get_wtime();
    runTime = endCalc - startCalc;
    printf("Calculated all %lu prime numbers under %lu in %f seconds with array.\n",prime_count, LIMIT, runTime);
    */


    prime_count = 0;
    startCalc = omp_get_wtime();
    //find_primes_array_parallel();  // 10000000
    find_primes_threaded();          // 10000000 in 0.447000
    endCalc = omp_get_wtime();
    runTime = endCalc - startCalc;
    printf("Calculated all %llu prime numbers under %llu in %f seconds with brute force in parallel.\n\n",prime_count, LIMIT, runTime);

    /*
    startCalc = omp_get_wtime();
    head = bubble_sort_list(head); // 1000000 in 5.466000
    endCalc = omp_get_wtime();
    runTime = endCalc - startCalc;
    printf("Sorted all %llu prime numbers under %llu in %f seconds.\n\n",prime_count, LIMIT, runTime);
    */

    //print_list(head);
    //head = getLastItem(head);
    printf("Largest prime number found: %lli\n\n", get_max_list(head));
    //print_list(head);

    //print_primes(out);
    //qsort(out, prime_count, sizeof(out[0]), cmpfunc);
    //print_primes(out3);



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
