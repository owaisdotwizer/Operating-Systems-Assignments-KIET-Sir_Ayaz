#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <pthread.h>

#define no_of_clients 100
#define size_of_clients 8
sem_t mutex;
struct shared_mem {
    char dest[8];
    char src[8];
    char message[239];

};

int running = 1;

void *inputs(void *args)  //thread for checking Quit for server
{
    char oo[7];
    while (running) {
        scanf("%s", oo);
        if (strcmp("@Quit", oo) == 0) { running = 0; }
    }
}

char arr2d[no_of_clients][size_of_clients];
int clients = 0;

void insert(char *clientid) {
    strcpy(arr2d[clients], clientid);
    clients++;
}

void print2darr() {
    printf("printing...\n");
    int i = 0;
    while (i != clients) {
        if (strlen(arr2d[i]) > 1) {
            printf("%s\n", arr2d[i]);
        }
        i++;
    }
}

int search2darr(char *phrase) {
    int i = 0;
    while (i != clients) {
        if (strcmp(phrase, arr2d[i]) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}


void delete(char *phrase) {
    int lineNo = search2darr(phrase);
    if (lineNo >= 0) {
        arr2d[lineNo][0] = '\0';
    }
}

void put_clients_in_msg(char *message) {
    int k = 0;
    message[0] = '\0';
    while (k != clients) {
        if (strlen(arr2d[k]) > 1) {
            strcat(message, arr2d[k]);
            strcat(message, "\n");
        }
        k++;
    }

}

int main() {

//printf("hahah");
    void *shareobj = (void *) 0;
    struct shared_mem *sharedstuff;
    int shmid;
    pthread_t pid;

    shmid = shmget((key_t) 1234, sizeof(struct shared_mem), 0666 | IPC_CREAT);  //creating shared memory

    shareobj = shmat(shmid, (void *) 0, 0);   //attaching shared memmory


    sharedstuff = (struct shared_mem *) shareobj;
    pthread_create(&pid, 0, inputs, (void *) 0);   //creating thread for checking messagebox
    sem_init(&mutex, 0, 1);    //initializing semaphores for synchronisation
    write(1, "Please Enter @Quit to End Server", 33);
    printf("\n");
    while (running) {
        if (strcmp(sharedstuff->dest, "server") == 0)   //maintaining list of clients
        {
            sem_wait(&mutex);
            memset(sharedstuff->dest, 0, 8);
            memset(sharedstuff->message, 0, 239);
            write(1, sharedstuff->src, 8);
            write(1, " Joined", 8);
            insert(sharedstuff->src);
            printf("\n");
            memset(sharedstuff->src, 0, 8);
            memset(sharedstuff->dest, 0, 8);
            memset(sharedstuff->message, 0, 239);
            sem_post(&mutex);
        }
        else if (strcmp(sharedstuff->dest, "AList") == 0)   //sending list of client to client
        {
            sem_wait(&mutex);
            memset(sharedstuff->message, 0, 239);
            memset(sharedstuff->dest, 0, 8);
            write(1, "list Requested By ", 19);
            write(1, sharedstuff->src, 8);
            printf("\n");
            strcpy(sharedstuff->dest, sharedstuff->src);
            put_clients_in_msg(sharedstuff->message);
            sem_post(&mutex);
        }
        else if (strcmp(sharedstuff->dest, "AQuit") == 0)   //deleting client who is not active any more
        {
            sem_wait(&mutex);
            memset(sharedstuff->dest, 0, 8);
            memset(sharedstuff->message, 0, 239);
            write(1, "Quit ", 5);
            write(1, sharedstuff->src, 8);
            printf("\n");
            delete(sharedstuff->src);
            strcpy(sharedstuff->src, "updat");
            memset(sharedstuff->dest, 0, 8);
            memset(sharedstuff->message, 0, 239);
            put_clients_in_msg(sharedstuff->message);
            sem_post(&mutex);
        }

    }

    pthread_join(pid, NULL);
    sem_destroy(&mutex);   //destroying semaphore
    shmdt(shareobj);  //deattaching shared memory
    shmctl(shmid, IPC_RMID, 0);
    exit(0);

}
