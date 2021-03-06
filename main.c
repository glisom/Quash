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

static int job_count = 0;
static struct Job jobs[100];
static char* env;
static char* dir; 

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
        dir = getcwd(NULL, 1024);
    } else {
        if (chdir(p) == -1) {
            printf(" %s: No such file or directory\n", strerror(errno));
        }
        dir = getcwd(NULL, 1024);
    }
}

int set_path(char* input) {
    char* setter = strtok(input, "=");
    char* ptype = setter;
    setter = strtok(NULL, "\0");
    char* path = setter;
    
    if ((setenv(ptype,path,1)) == - 1) {
        printf("%s was not correctly set.\n", ptype);
    }
    return 1;
    
}

void display_jobs() {
    int i;
    printf("\nActive jobs:\n");
    printf("| %7s  | %7s | %7s |\n", "Job ID", "PID  ", "Command");
    for (i=0; i < job_count; i++) {
        if (kill(jobs[i].pid, 0) == 0) {
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
            if (execvp(cmds[0], cmds) < 0) {
                fprintf(stderr, "That is an invalid command\n");
                exit(0);
            }
        } else {
            if (execvp(cmds[0], NULL) < 0) {
                fprintf(stderr, "That is an invalid command\n");
                exit(0);
            }
        }
    }
    else {
        waitpid(pid, &status, 0);
        if (status == 1) {
            fprintf(stderr, "%s\n", "Darn ,the pokemon fled...\n");
        }	
    }
}

void parse_cmd(char* input) {
    int i;
    char* command;
    int fout = 1994;
    char* args[20];
    for (i = 0; i < 20; i++) {
        args[i] = NULL;
    }
    int args_count = 0;
    char* cur_input = strdup(input);
    command = strtok(input, " ");
    while (command != NULL) {
        args[args_count] = command;
        command = strtok(NULL, " ");
        args_count++;
    }
    char* just_args[19];
    for (i = 0; i < 19; i++) {
        just_args[i] = NULL;
    }
    for (i = 1; i < 20; i++) {
        if (args[i] != NULL) {
            just_args[i - 1] = args[i];
        }
    }
    char* is_backgrd = strchr(cur_input, '&');
    char* bg_command = strdup(input);
    bg_command[strlen(input) - 1] = 0;
    char* is_pipe = strchr(cur_input, '|');
    char* filedir_in = strchr(cur_input, '<');
    char* filedir_out = strchr(cur_input, '>');
    char* kill_proc = strstr(cur_input, "kill");
    int change_dir = strcmp("cd", args[0]);
    int set = strcmp(args[0], "set");
    int jobs_com = strcmp(args[0], "jobs");
    
    if (change_dir == 0) {
        cd(args[1]);
    }
    else if (set == 0) {
        set_path(args[1]);
    }
    else if (jobs_com == 0) {
        display_jobs();
    }
    else if (is_backgrd != NULL) {
        int status;
        pid_t pid;
        pid_t sid; 
        pid = fork();
        if (pid == 0) {
            sid = setsid();
            if (sid < 0 ) {
                fprintf(stderr, "Could not create the new process\n");
                exit(0);
            }
            printf("New process with pid %d running out of %d processes\n", getpid(), job_count + 1);
            parse_cmd(bg_command);
            printf("The process with pid %d has finished\n", getpid());
            kill(getpid(), -9);
            exit(0);
        }
        else {
            struct Job new_job = {.pid = pid, .id = job_count, .cmd = bg_command};
            jobs[job_count] = new_job;
            job_count++;
            while(waitid(pid, NULL, WNOHANG | WEXITED) > 0) {}
        }
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
            parse_cmd(rm_whitespace(first_cmd));
            close(spipe[0]);
            close(spipe[1]);
            exit(0);
        }
        pid2 = fork();
        if (pid2 == 0) {
            dup2(spipe[0], STDIN_FILENO);
            parse_cmd(rm_whitespace(second_cmd));
            close(spipe[0]);
            close(spipe[1]);
            exit(0);
        }
    }
    else if (filedir_in != NULL) {
        char* cmd_f_file = NULL;
        char* new_args[20];
        int i;
        for (i = 0; i < 20; i++) {
            new_args[i] = NULL;
        }
        size_t len = 0;
        ssize_t read;
        FILE* file_d;
        int status;
        pid_t pid;
        pid = fork();
        if (strcmp("quash", args[0]) == 0) {
            file_d = fopen(args[2], "r");
            do {
                read = getline(&cmd_f_file, &len, file_d);
                if (pid == 0) {
                    parse_cmd(rm_whitespace(cmd_f_file));
                } else {
                    waitpid(pid, &status, 0);
                    if(status == 1) {
                        fprintf(stderr, "%s\n", "Darn ,the pokemon fled...\n");
                    }
                }
            } while (read != -1);
            fclose(file_d);
        } else {
            file_d = fopen(args[2], "r");
            int x = 0;
            int position = 0;
            while (position == 0) {
                if (strcmp("<",args[x]) == 0) {
                    position = x;
                }
                x++;
            }
            while ((read = getline(&cmd_f_file, &len, file_d)) == -1) {
                char* temp_i = strtok(cmd_f_file, " \0");
                args[x] = temp_i;
                x++;
                printf("%s\n",args[x]);
            }
            if (pid == 0) {
                execute(args);
            } else {
                waitpid(pid, &status, 0);
                if(status == 1) {
                    fprintf(stderr, "%s\n", "Darn ,the pokemon fled...\n");
                }
            }
        }
    }
    else if (filedir_out != NULL) {
        int i;
        int position = 0;
        while (position == 0) {
            if (strcmp(">",args[i]) == 0) {
                position = i;
            }
            i++;
        }
        int status;
        pid_t pid;
        pid = fork();
        if (pid == 0) {
            char* filename = args[position + 1];
            fout = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            dup2(fout, STDOUT_FILENO);
            char* n_input = strtok(input, ">");
            parse_cmd(rm_whitespace(n_input));
            close(fout);
            exit(0);
        } else {
            waitpid(pid, &status, 0);
            if(status == 1) {
                fprintf(stderr, "%s\n", "Darn ,the pokemon fled...\n");
            }
        }
    }
    else if (kill_proc != NULL) {
        int pid = (int) strtol(args[2], NULL, 0);
        int signal = (int) strtol(args[1], NULL, 0);
        printf("Killing pid %d with signal %d\n", pid, signal);
        kill(pid, -9);
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
    char* input, prompt[128];
    env = getenv("USER");
    dir = getcwd(NULL, 1024);
    job_count = 0;
    while (true) {
        snprintf(prompt, sizeof(prompt), "[%s:%s]$ ", env, dir);
        input = readline(prompt);
        input = rm_whitespace(input);
        if (strcmp("exit", input) != 0 && strcmp("quit", input) != 0) {
            if (strlen(input) > 1) {
                input = rm_whitespace(input);
                parse_cmd(input);
            }
        } else {
            break;
        }
        free(input);
    }

    return 0;
}
