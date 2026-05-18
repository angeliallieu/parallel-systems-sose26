#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#define QUEUE_NAME "/fork_queue"

int main() {

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;

    mqd_t mq = mq_open(
        QUEUE_NAME,
        O_CREAT | O_RDWR,
        0644,
        &attr
    );

    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }


    if (pid == 0) {

        char buffer[256];

        ssize_t bytes = mq_receive(
            mq,
            buffer,
            sizeof(buffer),
            NULL
        );

        if (bytes == -1) {
            perror("mq_receive");
            exit(1);
        }

        printf("Die Zauberkugel sagt: %s\n", buffer);

        mq_close(mq);

        exit(0);
    }


    else {

        char *antworten[5] = {
            "Heute wird ein schoener Tag.",
            "Heute lieber nicht blau tragen.",
            "Deine Glueckszahl fuer heute ist 7.",
            "Die Chancen stehen gut.",
            "Frag mich morgen nochmal."
        };

        srand(time(NULL));

        int zufall = rand() % 5;

        char message[256];

        snprintf(
            message,
            sizeof(message),
            "%s",
            antworten[zufall]
        );

        sleep(1);

        if (mq_send(
                mq,
                message,
                strlen(message) + 1,
                0) == -1) {

            perror("mq_send");
            exit(1);
        }

        printf("Das Univerum hat gesprochen \n");

        wait(NULL);

        mq_close(mq);

        mq_unlink(QUEUE_NAME);
    }

    return 0;
}