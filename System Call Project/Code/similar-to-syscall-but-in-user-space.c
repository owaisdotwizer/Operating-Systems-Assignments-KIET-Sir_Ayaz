#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>


int my_getLength_Fn(char *str) {
    int i = 0;
    //printf("my_getLength_Fn\n");
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

void my_to_lower_Fn(char *arr) {
    int i = 0;
    //printf("my_to_lower_Fn\n");
    while (arr[i] != '\0') {

        if (arr[i] >= 'A' && arr[i] <= 'Z') {
            arr[i] = arr[i] + 32;
        }
        i++;
    }
}

int contains(char *container, char *query) {
    printf("searching for %s in\n%s\n",query,container);
    int j = 0, k = 0;
    //printf("contains\n");
    while (container[j] != '\0') {
        if (container[j] == query[k]) {
	    printf("%c = %c\n",container[j], query[k]);
            k++;
        }
	else{
		k=0;
	}
        j++;
    }
    if (k == my_getLength_Fn(query)) {
        //printf("1\n");
        return 1;
    } else {
        //printf("0\n");
        return 0;
    }
}

void copy(char *dest, char *src) {
    //printf("copy\n");
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

int search_phrase(int fd, char *phrase, char *buffer, int line, int to_lower) {

    char arr[200];
    char *ptr_arr = arr;
    char arr2[200];
    char *ptr_phrase = arr2;

    int i = 0;
    int temp_line = 1;

    if (fd < 0) {
        return -2;
    }
    if (line < 0) {
        return -3;
    }
    if (to_lower != 0 && to_lower != 1) {
        return -4;
    }
    printf("Search system call running\n");
    copy(ptr_phrase, phrase);
    printf("phrase: %s\n", phrase);
    printf("Copied ptr_phrase: %s\n", ptr_phrase);
    if (to_lower == 0) {
        my_to_lower_Fn(ptr_phrase);
        printf("Lower ptr_phrase: %s\n", ptr_phrase);
    }

    while (read(fd, &buffer[i], 1) == 1) {
        if (buffer[i] == '\n' || buffer[i] == 0x0 || buffer[i] == '\0') {
            buffer[i] = 0;
            if (i != 0) {
                //buffer now has a line
                copy(ptr_arr, buffer);
                //printf("line: %s\n", ptr_arr);
                if (to_lower == 0) {
                    my_to_lower_Fn(ptr_arr);
                    //printf("Lowercase line: %s\n", ptr_arr);
                }
		printf("%s\n",ptr_phrase);
		printf("%s\n",ptr_arr);
                if (line == 0 && contains(ptr_arr, ptr_phrase)) {
                    //printf("buffer: %s\n", buffer);
                    return temp_line;
                } else if (line == temp_line && contains(ptr_arr, ptr_phrase)) {
                    //printf("buffer: %s\n", buffer);
                    return temp_line;
                }
                temp_line++;
            }
            i = 0;
            continue;
        }
        i++;
    }
    buffer[0] = '\0';
    return -1;
}

int main() {
    int fd = open("history.txt", O_RDONLY, 14);
    int lineNo = 0;
    int case_sensitivity = 0;
    printf("file desc = %d\n", fd);
    char *buffer = malloc(100);

    char arr[10];
    printf("Enter Your search phrase:\n");
    scanf("%s",arr);
    printf("Enter line No(Enter 0 to search all lines):\n");
    scanf("%d",&lineNo);
    printf("Turn case sensitivity on/off 0|1:\n");
    scanf("%d",&case_sensitivity);
    //int ret = search_phrase(file desc, search phrase, buffer for result
    //                          , line No >= 0, case sensitivity flag 0|1);
    int ret = search_phrase(fd, arr, buffer, lineNo, case_sensitivity);
    printf("Sys call returned: %d\n", ret);
    printf("Found: %s\n", buffer);
    return 0;
}









