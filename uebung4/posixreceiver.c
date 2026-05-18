// receiver.c
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>

#define QUEUE_NAME "/meine_queue"

int main() {
    mqd_t mq;
    char buffer[256];

    mq = mq_open(QUEUE_NAME, O_RDONLY);

    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    ssize_t bytes_read = mq_receive(
        mq,
        buffer,
        sizeof(buffer),
        NULL
    );

    if (bytes_read == -1) {
        perror("mq_receive");
        exit(1);
    }

    printf("Empfangen: %s\n", buffer);

    mq_close(mq);

    // Queue löschen
    // mq_unlink(QUEUE_NAME);

    return 0;
}