#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


//command of program a & b
// is stored in these
char progA[10], progB[10];

//output of program a & b will
//be stored in these
char outA[1000], outB[1000];

//stat is used as a boolean
int stat = 0;

//for splitting the command line args
void split(char cmd[])
{
  int i = 0;
  while (cmd[i] != '\0')
  {
    //if p or c found in array replace it by '|'
    if (((i + 1) < strlen(cmd) && cmd[i - 1] == ' ' && (cmd[i] == 'p' || cmd[i] == 'c') && cmd[i + 1] == ' '))
    {
      //set stat = 1 if c is found
      if(cmd[i] == 'c')
      {
        stat = 1;
      }

      //replacing index by |
      cmd[i] = '|';
    }
    i++;
  }
  i = 0;

  //now we split the array by '|'
  char *splitted = strtok(cmd, "|");//this returns all characters before '|'
  strcpy(progA, splitted);          //copy the splitted to global 
                                    //variable progA

  splitted = strtok(NULL, "|");     //this returns all characters after '|'
  strcpy(progB, splitted);          //copy the splitted to global 
                                    //variable progA

}//split Ends

//To convert command line arguments
//to a single array of characters
char * parseCmdLineArgs(char *argv[])
{
  int i = 1;
  static char cmd[30];
  
  while (argv[i] != '\0')
  {
    strcat(cmd,argv[i]);
    strcat(cmd," ");
    i++;
  }
  
  i = 0;
  return cmd;
}//parseCmdLineArgs Ends

int main(int argc, char *argv[])
{
  //condition to check if CLA is valid
  if(argc < 4)
  {
    printf("Invalid Command Line Arguments\n");
    exit(1);
  }

  char *cmdPtr,cmd[30];

  //get CLA as a single array
  cmdPtr = parseCmdLineArgs(argv);//pointer is returned
  strcpy(cmd,cmdPtr);             //copy pointer to an cmd array

  //run split method
  split(cmd);

  int fd[2];  //create 2 pipes
  int cd[2];

  pipe(fd);
  pipe(cd);

  pid_t pro1, pro2;

  pro1 = fork();

  if (pro1 == 0)//child 1
  {
    if(stat == 1)//then send output to parent
    {
      close(fd[0]);  //closing reading side of pipe
      dup2(fd[1], 1);  //duplicating stdout to pipe
      close(fd[1]);  //closing writing side
      system(progA);   // its output will stored in pipe
    }
    else  //then redirect output to next process
    {
      close(fd[0]);    //closing reading side of pipe
      dup2(fd[1], 1);    //duplicating stdout to pipe
      close(fd[1]);   //closing writing side
      system(progA);    // its output will stored in pipe

      return 0;//End child 1
    }
  }//child 1 Ends

  else  //parent
  {
    //creating another process in parent
    //for child 2
    pro2 = fork();

    if (pro2 == 0)//child 2
    {
      if(stat == 1)//then send output to parent
      {
        close(cd[0]); //closing reading side of pipe
        dup2(cd[1], 1);  //duplicating stdout to pipe 
        close(cd[1]); //closing writing side
        system(progB);	// its output will stored in pipe
      }
      else
      {
        close(fd[1]);  //closing writing side of pipe
        dup2(fd[0], 0);  //duplicating stdin to pipe 
        close(fd[0]);   //closing reading side
        system(progB); // it will take input from pipe
      }
    }

    //Parent process

    //if stat, then compare
    if(stat == 1)
    {
      read(fd[0],outA,1000);//read output of first program from pipe and save in array
      read(cd[0],outB,1000);//read output of second program from pipe and save in array

      //compare both arrays
      if(strcmp(outA,outB) == 0) //Comparing both outputs
      {
        printf("TRUE \n");
      }
      else
      {
        printf("FALSE \n");
        printf("\nCommand(%s) OUTPUT: \n",progA);
        printf("%s",outA);
        printf("\nCommand(%s) OUTPUT: \n",progB);
        printf("%s",outB);
      }
      return 0;// end program
    }
    else
    {
      wait(NULL);
      close(fd[1]);  //closing pipes
      close(fd[0]);   //closing pipes
      wait(NULL);
    }
  }
}
