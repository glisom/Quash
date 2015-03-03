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

int flag = 1;
char inputString[200];

struct job {
  char *command;
  int jobid;
  int pid;


};

void parse(char *input) {
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
    fgets (inputString, 200, stdin);
    trimwhitespace(inputString);
    if (strcmp(inputString, "exit") == 0 || strcmp(inputString, "quit") == 0) {
      flag = 0;
    }
    if (strcmp(inputString, "cd") == 0) {
      char *i;
      cd(i);
    }

  } while (flag);


  return(0);
}

