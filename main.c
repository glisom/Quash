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

struct job {
  char *command;
  int jobid;
  int pid;
};

void parse(char *input) {

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

  }

  if (strcmp("ls", command) ==0) {
    //ls();
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

 

  do {
    printf(":$ ");
    fgets(inputString, 200, stdin);
    trimwhitespace(inputString);
    command = strtok(inputString, " ");
    var1 = strtok(NULL, " ");
    printf("Command: %s\n", command);
    printf("First Variable: %s\n", var1);
    shellCommand();

  } while (flag);


  return(0);
}
