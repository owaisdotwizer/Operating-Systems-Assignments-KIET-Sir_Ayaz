#include <linux/kernel.h>
#include <linux/unistd.h>
#include <asm/unistd.h>
#include <linux/syscalls.h>

#include "myheaders.h"


//this function returns length of the argument string
int my_getLength_Fn(char *str) {
    int i = 0;
    printk("my_getLength_Fn\n");
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

//this function converts argument string to lowercase
void my_to_lower_Fn(char *arr) {
    int i = 0;
    printk("my_to_lower_Fn\n");
    while (arr[i] != '\0') {
        if (arr[i] >= 'A' && arr[i] <= 'Z') {
            arr[i] = arr[i] + 32;
        }
        i++;
    }
}

//this function checks if a line(container) contains a given string(query)
int contains(char *container, char *query) {
    int j = 0, k = 0;
    printk("contains\n");
    while (container[j] != '\0') {
        if (container[j] == query[k]) {
            k++;
        }
	else{
	    k = 0;
	}
        j++;
    }
    if (k == my_getLength_Fn(query)) {
        printk("1\n");
        return 1;
    } else {
        printk("0\n");
        return 0;
    }
}

//this function copy source string to destination string
void copy(char *dest, char *src) {
    printk("copy\n");
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

asmlinkage long sys_search_phrase(int fd, char *phrase, char *buffer, int line, int to_lower) {

    char arr[200];
    char *ptr_arr = arr;
    char arr2[200];
    char *ptr_phrase = arr2;
    int i = 0;
    int temp_line = 1;/*this var has info about the 
			line Number that is currently in buffer*/

//	checking for invalid arguments				   //
//-----------------------------------------------------------------//
    if (fd < 0) {
        return -2;
    }
    if (line < 0) {
        return -3;
    }
    if (to_lower != 0 && to_lower != 1) {
        return -4;
    }
//-----------------------------------------------------------------//

    //print message to kernel log
    printk("Search system call running\n");

    copy(ptr_phrase, phrase);
    if (to_lower == 0) {
        my_to_lower_Fn(ptr_phrase);
    }

    //this loop reads the file char by char
    while (sys_read(fd, &buffer[i], 1) == 1) {/*here char is read into buffer 
						because buffer is provide from user space
						and you cannot read into a kernel space var*/
        if (buffer[i] == '\n' || buffer[i] == 0x0 || buffer[i] == '\0') {
            buffer[i] = 0;
            if (i != 0) {   //buffer now has a line
			    //when a line has read into buffer
                copy(ptr_arr, buffer);//copy the line to a temp var
		
                if (to_lower == 0) {
                    my_to_lower_Fn(ptr_arr);
		    //converting string to lower case
                }

                if (line == 0 && contains(ptr_arr, ptr_phrase)) {
                    return temp_line;
                }
		else if (line == temp_line && contains(ptr_arr, ptr_phrase)) {
                    return temp_line;
                }
                temp_line++;//incrementing current line number
            }
            i = 0;
            continue;/*continue while loop without incrementing 'i'
			because i is set to 0 above*/
        }
        i++;
    }//while

    buffer[0] = '\0';/*if nothing is found put NULL in buffer
			because buffer will have last line of file right now*/
    return -1;
}
