#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>

int running = 1;
sem_t mutex;
char ok[8];
struct shared_mem {
    char dest[8];
    char src[8];
    char message[239];
};

void *input(struct shared_mem *share)   //thread for checking is the message is for me
{
    int b = 0;
    while (running) {
        if (strcmp(share->src, "updat") == 0 && b == 0) {
            write(1, "Updated List", 13);
            printf("\n");
            write(1, share->message, 239);
            b = 1;
        }
        if (strcmp(share->dest, ok) == 0) {
            sem_wait(&mutex);
            write(1, share->message, 239);
            memset(share->dest, 0, 8);
            memset(share->message, 0, 239);
            memset(share->src, 0, 8);
            sem_post(&mutex);

            printf("\n");
        }


    }
}

void *alive(struct shared_mem *share) {   //notify server in every 10 sec that this pid or client is alive
    while (running) {
        sleep(10);

        strcpy(share->message, "alive");
        strcpy(share->dest, "server");
        snprintf(share->src, 8, "%d", getpid());

    }

}

int main() {

    void *shareobj = (void *) 0;
    struct shared_mem *sharedstuff;
    int shmid;
    pthread_t pid;

    shmid = shmget((key_t) 1234, sizeof(struct shared_mem), 0666 | IPC_CREAT);  //creating shared memory

    shareobj = shmat(shmid, (void *) 0, 0);   //attaching shared memory


    sharedstuff = (struct shared_mem *) shareobj;


    snprintf(ok, 8, "%d", getpid());

    strcpy(sharedstuff->dest, "server");   //advertising msg to server for adding to list
    strcpy(sharedstuff->src, ok);

    pthread_create(&pid, 0, input, sharedstuff);  //creating thread for message checking
    sem_init(&mutex, 0, 1);   //initializing semaphore for synchronisation
    char ab[8];
    char bb[20];
    write(1, "Please Enter @List for list,@Quit for endyoursession,clientid message for sending a message", 92);
    printf("\n");
    while (running) {

        scanf("%s", ab);
        if (ab[0] != '@') {
            scanf("%[^\n]%*c", bb);
        }


        if (ab[0] == '@') { ab[0] = 'A'; }

        if (strcmp(ab, "AQuit") == 0) { running = 0; }  //Taking input and sending to specif client or to server
        sem_wait(&mutex);
        strcpy(sharedstuff->dest, ab);
        strcpy(sharedstuff->src, ok);
        strcpy(sharedstuff->message, bb);
        sem_post(&mutex);
    }

    pthread_join(pid, NULL);
    shmdt(shareobj);  //deattaching shared memory
    sem_destroy(&mutex);   //destroying Semaphores
    exit(0);
}
