#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>

int main(int argc, char **argv char **envp) {

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
