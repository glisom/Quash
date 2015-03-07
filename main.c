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
#define false 0

struct Job {
    int pid;
    int id;
    char* cmd;
};

static int job_count;
static struct Job jobs[20];

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

int setPath(char* input) {
    char* setter = strtok(input, "=");
    char* ptype = setter;
    setter = strtok(NULL, "\0");
    char* path = setter;
    
    if((setenv(ptype,path,1)) == - 1) {
        printf("%s was not correctly set.\n", ptype);
    }
    return 1;
    
}

void displayJobs() {
    int i;
    printf("\nActive jobs:\n");
    printf("| %7s  | %7s | %7s |\n", "Job ID", "PID  ", "Command");
    for(i=0; i < job_count; i++) {
        if(kill(jobs[i].pid, 0) == 0) {
            printf("|  [%7d] | %7d | %7s |\n", jobs[i].id, jobs[i].pid,
                   jobs[i].cmd);
        }
    }
}

void execute(char** cmds) {
    int status;
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        if (strlen(cmds[0]) > 0) {
            if(execvp(cmds[0], cmds) < 0) {
                fprintf(stderr, "That is an invalid command\n");
                exit(0);
            }
        } else {
            if(execvp(cmds[0], NULL) < 0) {
                fprintf(stderr, "That is an invalid command\n");
                exit(0);
            }
        }
    }
    else {
        waitpid(pid, &status, 0);
        if(status == 1) {
            fprintf(stderr, "%s\n", "Darn ,the pokemon fled...\n");
        }	
    }
}

void parse_cmd(char* input) {
    char* command;
    char* args[20];
    for (int i = 0; i < 20; i++) {args[i] = NULL;}
    int args_count = 0;
    char* cur_input = strdup(input);
    command = strtok(input, " ");
    while (command != NULL) {
        args[args_count] = command;
        command = strtok(NULL, " ");
        args_count++;
    }
    char* just_args[19];
    for (int i = 0; i < 19; i++) {just_args[i] = NULL;}
    for (int i = 1; i < 20; i++) {if (args[i] != NULL) {just_args[i - 1] = args[i];}}
    char* is_backgrd = strchr(cur_input, '&');
    char* is_pipe = strchr(cur_input, '|');
    char* filedir_in = strchr(cur_input, '<');
    char* filedir_out = strchr(cur_input, '>');
    int change_dir = strcmp("cd", args[0]);
    int set = strcmp(args[0], "set");
    int jobs = strcmp(args[0], "jobs");
    
    if (change_dir == 0) {
        cd(args[1]);
    }
    else if (set == 0) {
        setPath(args[1]);
    }
    else if (jobs == 0) {
        displayJobs();
    }
    else if (is_backgrd != NULL) {
        
    }
    else if (is_pipe != NULL) {
        char* part = strtok(cur_input, "|\0");
        char* first_cmd = part;
        part = strtok(NULL, "\0");
        char* second_cmd = part;
        int spipe[2];
        int status;
        pipe(spipe);
        pid_t pid;
        pid_t pid2;
        pid = fork();
        if (pid == 0) {
            dup2(spipe[1], STDOUT_FILENO);
            close(spipe[0]);
            close(spipe[1]);
            parse_cmd(rm_whitespace(first_cmd));
            exit(0);
        }
        pid2 = fork();
        if (pid2 == 0) {
            dup2(spipe[0], STDIN_FILENO);
            close(spipe[0]);
            close(spipe[1]);
            parse_cmd(rm_whitespace(second_cmd));
            exit(0);
        }
    }
    else if (filedir_in != NULL) {
        
    }
    else if (filedir_out != NULL) {
        
    }
    else {
        execute(args);
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
    rl_bind_key('\t', rl_complete);
    int flag = true;
    char* input, prompt[128];
    do {
        snprintf(prompt, sizeof(prompt), "[%s:%s]$ ", getenv("USER"), getcwd(NULL, 1024));
        input = readline(prompt);
        add_history(input);
        input = rm_whitespace(input);
        if (strcmp("exit", input) != 0 && strcmp("quit", input) != 0) {
            if (strlen(input) > 1) {
                input = rm_whitespace(input);
                parse_cmd(input);
            }
        } else {
            flag = false;
        }
        free(input);
    } while (flag);

    return 0;
}
