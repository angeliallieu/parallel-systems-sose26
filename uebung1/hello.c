#include <stdio.h>
#include <unistd.h>

int hello() {
    printf("Hello World!\n");
    return 0;
}


int forking() {
    fork();  // erster Fork
    fork();  // zweiter Fork

    printf("Ich bin Prozess mit PID: %d\n", getpid());
    return 0;
}

int main() {
    hello();
    forking();
    return 0;
}
