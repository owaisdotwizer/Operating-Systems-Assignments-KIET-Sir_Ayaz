#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <termio.h>

#define cmdLength 1000 //maximum command length
int count = 0;
int line = 0; //global varibles used by multiple methods
int bool = 0, bool2 = 1, bool3 = 0; //using integers as boolean
char cmdByName = NULL;
char *cmd1;
int globalLine = 0;


//To read key events
int readKey()
{

  char choice[4];
  FILE *input;
  FILE *output;
  struct termios initial_settings, new_settings;

  if (!isatty(fileno(stdout)))
  {
    fprintf(stderr, "You are not a terminal, OK.\n");
  }

  input = fopen("/dev/tty", "r");
  output = fopen("/dev/tty", "w");
  if (!input || !output)
  {
    fprintf(stderr, "Unable to open /dev/tty\n");
    exit(1);
  }

  tcgetattr(fileno(input), &initial_settings);
  new_settings = initial_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  new_settings.c_cc[VMIN] = 1;
  new_settings.c_cc[VTIME] = 0;
  new_settings.c_lflag &= ~ISIG;

  if (tcsetattr(fileno(input), TCSANOW, &new_settings) != 0)
  {
    fprintf(stderr, "could not set attributes\n");
  }

  read(0, choice, 4);
  if (choice[0] == '\33')
  {
    if (choice[2] == 'A')//if up arrow key
    {
      //change globalLine + 1 which will be passed in readerByLineNo(int lineNo)
      globalLine+=1;
      return 1;//this will be used in an if condition
    }
    else if (choice[2] == 'B')
    {
      //change globalLine - 1 which will be passed in readerByLineNo(int lineNo)
      globalLine-=1;
      return 1;//this will be used in an if condition
    }
  }

  tcsetattr(fileno(input), TCSANOW, &initial_settings);
  return 0;
}

void reader(int lineNo)
{
  int cmd1i = 0; //iteration variable
  int fd, bytes, count = 0, lineSize = 100, i = 0;
  char *buffer;
  buffer = (char *)malloc(lineSize * sizeof(char));
  cmd1 = (char *)malloc(1000 * sizeof(char));
  int icopy = 0, icopykicopy = 0;
  char *copy = (char *)malloc(100);
  char c;
  fd = open("history.txt", O_RDONLY, 0);

  while (read(fd, &buffer[i], 1) == 1) //reading one character at a time
  {
    if (buffer[i] == '\n' || buffer[i] == 0x0) 
    //compare character with null or carriage return
    {
      buffer[i] = 0;
      if (i != 0)//if i is != 0 means buffer has read a line
      {
        if (buffer[0] == cmdByName) //condition for !gcc
        {
          if (bool2)
          {
            //if true copy line in a 
            //global variable cmd1
            while (cmd1i <= i)
            {
              cmd1[cmd1i] = buffer[cmd1i];
              cmd1i++;
            }
            bool2 = 0; //set bool back to false
            line = 0;
            break; // no more need for a loop
          }
          else //else keep the line if command is !!gcc
          {
            while (icopy <= i)
            {
              copy[icopy] = buffer[icopy];
              icopy++;
            }
            icopykicopy = icopy;
            icopy = 0;
          }
        }
        if (line == lineNo)   //condition to get command by number
                              //this is for arrow keys
        {
          while (cmd1i <= i) //copy line to global variable cmd1
          {
            cmd1[cmd1i] = buffer[cmd1i];
            cmd1i++;
          }
          break;
        }
        line++; // if no condition is 
      }         // true increment line
      i = 0; //set i = 0 to change line
      continue; //run loop again without incrementing i
    }
    i++;
  }
  close(fd); // close the file
  if (bool3) //condition for !!gcc
  {
    //above we kept the line if bool2 was false
    while (cmd1i <= icopykicopy)
    {
      cmd1[cmd1i] = copy[cmd1i];
      cmd1i++;
    }
    bool3 = 0;
    //set bool3 false so it doesnt get triggered again
  }
}


//this method is use for arrow keys pressing up will run this method
//with 1 as an argument and keep inc or dec the argument
//based on the key pressed. This metthod calls reader()
// arg 1 will get the last line
void readerByLineNo(int reverseLineNo)
{
  if (bool)//this condition is only used when another 
  {         //method below calls this method
    bool = 0;
    line = 0;
    //arg should never be less than 0
    (reverseLineNo > -1) ? (reader(reverseLineNo)) : (NULL);
  }
  else
  {
    line = 0;
    reader(-2); // run method with a useless arg just to get total number of lines
    int temp = line - reverseLineNo;
    line = 0;
    (temp > -1) ? (reader(temp)) : (NULL);
  }
}


//this method is used to get lines when 
//user input begins with ! or !!
void readerByExclaimationMarks(char arg[], int arglen)
{
  int sizeofarg = arglen, i = sizeofarg - 1;
  int lineArr[] = {0, 0, 0};
  //shell will not support more than 999 lines of history
  int i2 = 2, i3 = 0;
  int value = 0;

  if (arg[1] == '!')//if second char is '!'
  {
    //it is reverse command
    if ((arg[2] >= 'a' && arg[2] <= 'z') || (arg[2] >= 'A' && arg[2] <= 'Z'))
    //check if it is an alphabet
    {
      cmdByName = arg[2];
      bool2 = 0;
      bool3 = 1;
      reader(-2);
      goto l1; //if this condition is ture
              // nothing below is needed
    }
  }
  else if (arg[0] == '!' && arg[1] != '!')
  //if there is only one '!'
  {
    //it is in sequence
    if ((arg[1] >= 'a' && arg[1] <= 'z') || (arg[1] >= 'A' && arg[1] <= 'Z'))
    //check if it is an alphabet
    {
      bool2 = 1;
      bool3 = 0;
      cmdByName = arg[1];
      reader(-2);
      goto l1;//if this condition is ture
              // nothing below is needed
    }
  }


  //if there is no alphabet after '!' it might be
  //a number so convert that char to a number
  while (arg[i] != '!')
  {
    ((arg[i] - '0') < 0) ? (lineArr[i2] = 0) : (lineArr[i2] = (arg[i] - '0'));
    i--;
    i2--;
  }
  //when chars are converted to int they are on 
  //seperate indexes so join them
  value = ((lineArr[0] * 10) * 10) + (lineArr[1] * 10) + lineArr[2];
      //multiply index 0 by 100 {add} multiply index 1 by 10 {add} index 2
      //unit,ten,hundred rule
  if (arg[1] == '!')
  {
    readerByLineNo(value + 1);
  }
  else if (arg[0] == '!' && arg[1] != '!')
  {
    bool = 1;//set bool true to tell readerByLineNo
            //to get line from the top
    readerByLineNo(value);
  }
l1:; //terminator mean an empty statement
      // without this label will not work
}

//this methods takes all input and returns a pointer to be executed
char *takeInput()
{
  char *buf[1];
  char *cmd = (char *)malloc(sizeof(char) * cmdLength);
  while (1)
  {
    read(0, buf, 1);
    //if enter is pressed break loop
    if (*(buf + 0) == '\n')
    {
      break;
    }
    //if readKey returns 1 the read from history
    else if(readKey() == 1){
      readerByLineNo(globalLine);
    }
    //save all input chars in *cmd
    *(cmd + (count++)) = *(buf + 0);
  }
  count = 0;
  if ((*cmd + 0) == '!')//if first char is ! 
                        //then call readerByExclaimationMarks()
  {
    //get length of cmd
    int length = 0;
    while (*cmd != '\0')
    {
      length++;
      cmd++;
    }
    cmd -= length;
    //get length of cmd Ends

    readerByExclaimationMarks(cmd, length);
    cmd = cmd1;
    write(0, "CS_202_Shell > ", 15);//after taking the input print shell name again
    write(0, cmd,strlen(cmd1));    // and print history command
  }
  return cmd;
}

void saveInput(char *input)
{
  int fd, count = 0;
  // open or creat file with read write and append
  //means you can also get history after you exit
  fd = open("history.txt", O_CREAT | O_RDWR | O_APPEND, 0666);
  if (fd < 0)//if file is not open give error
  {
    write(1, "There is an error opening the file", 35);
  }
  else
  {
    write(fd, input, sizeof(input));//write command
    write(fd, "\n", 2);//change line
  }
}
close(fd);

int main(void)
{
  while (1)
  {
    int pid;
    int cont = 0;
    char *args[10];//this will point to another pointer of string
    char *cmd;//it has user input
    char *cmdToArray = (char *)malloc(cmdLength * cmdLength * sizeof(char));
    //to split user input and save an array of strings

    //Main Flow
    write(1, "CS_202_Shell > ", 15);//write terminal name
    cmd = takeInput();
    saveInput(cmd);

    int ctr = 0, j = 0, i;
    if (*(cmd + 0) != '\0')//check char array if it is not null
    {
      for (i = 0; i < cmdLength; i++)//cmdLength = 1000
      {
        // if space or NULL found, assign NULL into newString[ctr]
        if (*(cmd + i) == ' ' || *(cmd + i) == '\0')
        {
          *(cmdToArray + ctr * cmdLength + j) = '\0'; //changing index with a NULL
          ctr++; //for next word
          j = 0; //for next word index will be 0 like a 2d array
        }
        // if space or NULL is not found copy cmd indexes to cmdToArray
        else
        {
          *(cmdToArray + ctr * cmdLength + j) = *(cmd + i);
          j++;
        }
      }

      for (i = 0; i <= ctr; i++) // traverse through array of strings(cmdTo Array)
      {
        if (*(cmdToArray + i * cmdLength) != '\0' && *(cmdToArray + i * cmdLength) != ' ')
        {
          args[i] = cmdToArray + i * cmdLength; //creating a pointer to pointer
          cont++;
        }
      }

      args[cont] = NULL;//add null at the very end also
      pid = fork();//create another process because
                  //all exec commands replace the process
                  //they are inside
      if (pid == 0)
      {
        if (execvp(args[0], args) < 0)//to handle invalid commands
        {
          write(1, "Invalid command \n", 18);
        }
      }
      wait(NULL);
      pid = 0;
    }
  }
}
