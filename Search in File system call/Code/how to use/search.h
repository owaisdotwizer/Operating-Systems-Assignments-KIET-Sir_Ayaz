#include <sys/syscall.h> //Including system calls Header File to call our custom system call

#define SYS_search_phrase 326  //Defining our system call n=umber

long search(int fd, char *phrase, char *buffer, int line, int to_lower){    //Wrapper for our system calls
	return syscall(SYS_search_phrase, fd, phrase, buffer, line, to_lower);   //Defining our system calls and returning it
}
