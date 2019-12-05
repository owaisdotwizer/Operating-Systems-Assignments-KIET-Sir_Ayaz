#include <sys/syscall.h>

#define SYS_search_phrase 326

long search(int fd, char *phrase, char *buffer, int line, int to_lower){
	return syscall(SYS_search_phrase, fd, phrase, buffer, line, to_lower);
}
