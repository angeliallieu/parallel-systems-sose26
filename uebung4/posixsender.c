// sender.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define QUEUE_NAME "/meine_queue"

int main() {
    mqd_t mq;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;

    // Queue erstellen oder öffnen
    mq = mq_open(
        QUEUE_NAME,
        O_CREAT | O_WRONLY,
        0644,
        &attr
    );

    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    char message[] = "Le cheval ne mange pas de salade des concombres!";

    if (mq_send(mq, message, strlen(message) + 1, 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    printf("Nachricht gesendet\n");

    mq_close(mq);

    return 0;
}