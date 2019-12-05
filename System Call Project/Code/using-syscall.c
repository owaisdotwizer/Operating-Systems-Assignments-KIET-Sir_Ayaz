#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define SYS_search_phrase 326

int main()
{
	int fd = open("history.txt", O_RDONLY);
	int lineNo = 0;
	int case_sensitivity = 0;
	char *buffer = malloc(100);
	char phrase[10];
	printf("Enter Your search phrase:\n");
	scanf("%s",phrase);
	printf("Enter line No(Enter 0 to search all lines):\n");
	scanf("%d",&lineNo);
	printf("Turn case sensitivity on/off 1|0:\n");
	scanf("%d",&case_sensitivity);
	long int ret = syscall(SYS_search_phrase, fd, phrase, buffer, lineNo, case_sensitivity);
	printf("Sys call returned: %ld\n",ret);
	printf("%s\n", buffer);
	return 0;
}
