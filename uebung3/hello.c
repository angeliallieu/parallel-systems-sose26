#include <omp.h>
#include <stdio.h>

int hello() {
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Hello World from Thread %d\n", tid);
    }
    
}

int main() {
    hello();
    return 0;
}
