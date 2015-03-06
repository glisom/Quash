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
#include <readline/readline.h>
#include <readline/history.h>
#define true 1

char* rm_whitespace(char* str) {
    char *end;

    while (isspace(*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = 0;

    return str;
}

void cd(char *p) {
    if (p == NULL) {
        chdir(getenv("HOME"));
    } else {
        if (chdir(p) == -1) {
            printf(" %s: No such file or directory\n", strerror(errno));
        }
    }
}

void quashPipe(char* cmd, char* cmd2) {
    int spipe[2];
    int status;
    pipe(spipe);
    pid_t pid;
    pid = fork();
    
    if (pid == 0) {
        dup2(spipe[1], STDOUT_FILENO);
        close(spipe[0]);
        close(spipe[1]);
        parse(rm_whitespace(cmd));
        exit(0);
    } else {
        dup2(spipe[0], STDIN_FILENO);
        close(spipe[0]);
        close(spipe[1]);
        waitpid(pid, &status, 0);
        parse(rm_whitespace(cmd2));
    }
}

int setPath(char* input) {
    char* ptype = strtok(input, '=');
    char* path = strtok(NULL, '\0');
    
    if((setenv(ptype,path,1)) ==- 1) {
        printf("%s was not correctly set.\n", ptype);
    }
    else {
        printf("set command was not understood.\n");
    }
    
    return 1;
    
}

void displayJobs() {
    printf("The pokemon got away...");
}

void parse_cmd(char* input) {
    char* command;
    char* args[20];
    int args_count = 0;
    char* cur_input = strdup(input);
    command = strtok(input, " ");
    args[args_count] = strtok(NULL, " ");
    while (args[args_count] != NULL) {
        args[args_count] = strtok(NULL, " ");
        args_count++;
    }
    char* is_backgrd = strchr(cur_input, '&');
    char* is_pipe = strchr(cur_input, '|');
    char* filedir_in = strchr(cur_input, '<');
    char* filedir_out = strchr(cur_input, '>');
    int change_dir = strcmp(command, "cd");
    int quit = strcmp(command, "quit");
    int exit = strcmp(command, "exit");
    int set = strcmp(command, "set");
    int jobs = strcmp(command, "jobs");
    
    if (change_dir) {
        cd(args[0]);
    }
    if (quit || exit) {
        exit(EXIT_SUCCESS);
    }
    if (set) {
        setPath(args[0]);
    }
    if (jobs) {
        displayJobs();
    }
    if (is_backgrd != NULL) {
        
    }
    if (is_pipe != NULL) {
        char* first_cmd = strtok(cur_input, "|\0");
        char* second_cmd = strtok(NULL,"\0");
        quashPipe(first_cmd, second_cmd);
    }
    if (filedir_in != NULL) {
        
    }
    if (filedir_out != NULL) {
        
    }
    else {
        if (strlen(cur_input) > 0) {
            if(execvp(command, args) < 0) {
                fprintf(stderr, "That is an invalid command\n");
                exit(0);
            }
        } else {
            if(execvp(command, NULL) < 0) {
                fprintf(stderr, "That is an invalid command\n");
                exit(0);
            }
        }
    }
    
}


int main(int argc, char** argv, char** envp) {
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃███████*███████████████████████████████████*████████████████████┃\n");
    printf("┃█*████████████████████████████████████████████████*█████████████┃\n");
    printf("┃█████*██████████████████████████████████████████████████████████┃\n");
    printf("┃█*██████*******████**█████**█████**██████********█**██████**████┃\n");
    printf("┃███████**/////**██/**████/**████****████**//////█/**█████/**████┃\n");
    printf("┃██████**█████//**█/**████/**███**//**██/**███████/**█████/**████┃\n");
    printf("┃█████/**██████/**█/**████/**██**██//**█/*********/**********████┃\n");
    printf("┃█████/**████**/**█/**████/**█**********////////**/**//////**████┃\n");
    printf("┃█████//**██//█**██/**████/**/**//////**███████/**/**█████/**████┃\n");
    printf("┃██████//*******█**//*******█/**█████/**█********█/**█████/**████┃\n");
    printf("┃███████///////█//██///////██//██████//█////////██//██████//█████┃\n");
    printf("┃████████████████████████████████████████████████████████████████┃\n");
    printf("┃█████*██████ Written by Alek Eskilison and Grant Isom █████*████┃\n");
    printf("┃██*█**████████████████████████████████████████████████████*█████┃\n");
    printf("┃████████████████████████████████████████████████████████████████┃\n");
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");

    char* input, prompt[128];
    do {
        snprintf(prompt, sizeof(prompt), "[%s:%s]$ ", getenv("USER"), getcwd(NULL, 1024));
        input = readline(prompt);
        add_history(input);
        if (strcmp("exit", input) != 0) {
            input = rm_whitespace(input);
            parse_cmd(input);
        }

    
    } while (true);

    return 0;
}
