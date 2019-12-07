
int my_getLength_Fn(char * str);
void my_to_lower_Fn(char * arr);
int contains(char *container, char *query);
void copy(char *dest, char *src);
asmlinkage long sys_search_phrase(int fd, char *phrase, char *buffer, int line, int to_lower);
