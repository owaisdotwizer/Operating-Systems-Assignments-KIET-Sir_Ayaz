#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void *Message(void *vargp)
{
   int fd2;          //file descriptor
   char command[15]; //command will be stored to be executed on client
   char *clientinfo; //Client name will be stored
   while (1)
   {
      fgets(command, 15, stdin); //taking command input
      gets(clientinfo);
      fd2 = open(clientinfo, O_RDWR);           //opening client fifo
      write(fd2, command, strlen(command) + 1); //writing commands to client fifos
      close(fd2);
   }
}

int main()
{
   pthread_t thread_id; //creating thread for server to update client list parallel with sending command
   int fd;
   char *fifos = "server123";
   mkfifo(fifos, 0666); //making server fifo
   char arr[15];        // client info will be saved
   printf("Enter Command, Press Enter, then Clientname \n");
   pthread_create(&thread_id, NULL, Message, NULL); //Staring thread , message() method will start executing from here
   while (1)
   {

      fd = open(fifos, O_RDONLY); //opening fifo
      read(fd, arr, sizeof(arr)); //taking clientinfo from pipe
      printf("Client Available: %s\n", arr);
      close(fd);
   }
   pthread_join(thread_id, NULL); //termination of thread

   return 0;
}
