#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>
#include <termios.h>

int flag = 1;
char inputString[200];
char *command;
char *var1;
static pid_t QuashPid;
static pid_t QuashPGid;
static int QuashTerminal;

int fileOut,fileIn;

struct job {
  char *command;
  int jobid;
  int pid;
};

void parse(char *input) {

}

int setPath() {
    int i, temp, len;
    i = 0;
    len = 0;
    char *pathcmd = (char *)malloc(sizeof(char)*50);
    /*for (int i =0; i<50; i++) {
        pathcmd[i] = '\0';
        i++;
    }*/
    char *path = var1;
    len = strlen(var1);

    if(strncmp(path, "HOME=", 5) == 0) {
        strncpy(pathcmd, path + 5, (len - 5));
        printf("Home is set to: %s, \n", pathcmd);
        if((setenv("HOME",pathcmd,1)) ==- 1){
            printf("Home was not correctly set.\n");
        }
    } else if(strncmp(path, "PATH=", 5) == 0) {
        strncpy(pathcmd, path + 5, (len - 5));
        printf("Path is set to: %s, \n", pathcmd);
        if((setenv("PATH",pathcmd,1)) ==- 1){
            printf("Path was not correctly set.\n");
        }
    } else {
        printf("set command was not understood.\n");
    }
    
    return 1;

}

void execute(char *input) {
  int curr_input = strdup(input);
  
}

void cd(const char *p) {
  if (p == NULL) {
    chdir(getenv("HOME"));
  } else {
    if (chdir(p) == -1) {
      printf(" %s: No such file or directory\n", strerror(errno));
    }
  }
}

void showActiveJobs() {

}

void shellCommand() {
  if (strcmp("exit", command) == 0 || strcmp("quit", command) == 0) {
    flag = 0;
  }

  if (strcmp("cd", command) == 0) {
    cd(var1);
  }

  if (strcmp("jobs", command) == 0) {
    showActiveJobs();
  }

  if (strcmp("set", command) == 0) {
    int setter = setPath();
  }

  //if (strcmp("ls", command) ==0) {
    //ls();??
  //}
    
  else {
      int position = 0;
      char *filename = NULL;
      filename = var1;
      //command[strlen(command -1)] = '\0';
      fileIn = open(filename, O_RDONLY);
      dup2(fileIn, STDIN_FILENO);
      close(fileIn);
      if (execvp(*command, command) == -1)
          perror("quash");
      close(fileIn);
      exit(0);
  }
}

char *trimwhitespace(char *str)
{
    char *end;
    // Trim leading space
    while(isspace(*str)) str++;
      //
      if(*str == 0)  // All spaces?
      return str;
      // Trim trailing space
      end = str + strlen(str) - 1;
      while(end > str && isspace(*end)) end--;
      // Write new null terminator
      *(end+1) = 0;
      return str;
}

int main(int argc, char **argv, char **envp) {

  //Pretty intro
  printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
  printf("┃████████████████████████████████████████████████████████████████┃\n");
  printf("┃████████████████████████████████████████████████████████████████┃\n");
  printf("┃████████████████████████████████████████████████████████████████┃\n");
  printf("┃████████*******████**█████**█████**██████********█**██████**████┃\n");
  printf("┃███████**/////**██/**████/**████****████**//////█/**█████/**████┃\n");
  printf("┃██████**█████//**█/**████/**███**//**██/**███████/**█████/**████┃\n");
  printf("┃█████/**██████/**█/**████/**██**██//**█/*********/**********████┃\n");
  printf("┃█████/**████**/**█/**████/**█**********////////**/**//////**████┃\n");
  printf("┃█████//**██//█**██/**████/**/**//////**███████/**/**█████/**████┃\n");
  printf("┃██████//*******█**//*******█/**█████/**█********█/**█████/**████┃\n");
  printf("┃███████///////█//██///////██//██████//█////////██//██████//█████┃\n");
  printf("┃████████████████████████████████████████████████████████████████┃\n");
  printf("┃████████████ Written by Alek Eskilison and Grant Isom ██████████┃\n");
  printf("┃████████████████████████████████████████████████████████████████┃\n");
  printf("┃████████████████████████████████████████████████████████████████┃\n");
  printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");

  QuashPid = getpid();
  QuashTerminal = STDIN_FILENO;

  if(isatty(QuashTerminal)) {
    while (tcgetpgrp(QuashTerminal) != (QuashPGid = getpgrp()))
                              kill(QuashPid, SIGTTIN);
  
  } else {
      printf("Quash is not in a terminal enviroment.\n Quitting...\n");
  }

  do {
    printf(":$ ");
    fgets(inputString, 200, stdin);
    trimwhitespace(inputString);
    command = strtok(inputString, " ");
    var1 = strtok(NULL, " ");
    shellCommand();

  } while (flag);


  return(0);
}
