#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 40

int main() {
    srand(time(NULL));

    int colors[N];
    int values[N];

    for (int i = 0; i < N; i++) {
        colors[i] = rand() % 3;      // 0-2
        values[i] = (rand() % 20) + 1; // 1-20
    }

    int sum[3] = {0};

    for (int i = 0; i < N; i++) {
        sum[colors[i]] += values[i];
    }

    for (int i = 0; i < 3; i++) {
        printf("Farbe %d Summe: %d\n", i, sum[i]);
    }

    return 0;
}