#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int fd1;
	int fd2; //file descriptors
	char ab[15];
	char *srvrcmd = ab; //server command will be stored
	char *serverfifo = "server123";
	char cl_fifo[15];							//clientinfo
	snprintf(cl_fifo, 15, "client%d", getpid()); // concatinating process id with client
	printf("%s\n", cl_fifo);
	mkfifo(cl_fifo, 0666); //fifo will be create as client+processid

	fd1 = open(serverfifo, O_WRONLY); //opening server fifo
	write(fd1, cl_fifo, strlen(cl_fifo) + 1); // writing client's fifo name to server's fifo
	close(fd1);
	fd2 = open(cl_fifo, O_RDWR); //open client fifo
	while (1)
	{
		read(fd2, srvrcmd, 16); //reading client fifo and checking for data sent by server
		printf("%s\n", srvrcmd); //printing recieved data
		system(srvrcmd); //executiong server coomand
	}
	close(fd2);

	return 0;
}
