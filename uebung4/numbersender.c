// sender.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include<unistd.h>

#define QUEUE_NAME "/number_queue"

void delay(int number_of_seconds)
{
	// Converting time into milli_seconds
	int milli_seconds = 1000 * number_of_seconds;

	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds)
		;
}

int main() {

    mqd_t mq;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;

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

    double lower_bound = 0.0;
    double upper_bound = 100.0;

    srand(time(NULL));

    // Zahl in String umwandeln
    char message[256];

    // Fünfmal eine Zufallszahl erzeugen und senden
    for (int i = 0; i < 5; ++i) {
        double random_double =
            lower_bound +
            ((double)rand() / RAND_MAX)
            * (upper_bound - lower_bound);

        printf("Zufallszahl: %f\n", random_double);

        sleep(5); // Kurze Pause zwischen den Sendungen

        snprintf(
            message,
            sizeof(message),
            "%f",
            random_double
        );

        // Nachricht senden
        if (mq_send(
                mq,
                message,
                strlen(message) + 1,
                0) == -1) {

            perror("mq_send");
            exit(1);
        }
    }

    printf("Nachricht gesendet\n");

    mq_close(mq);

    return 0;
}