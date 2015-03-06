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


int flag = 1;
int status;
char inputString[200];
char* command;
char* args[10];
char* var1;
static pid_t QuashPid;
static pid_t QuashPGid;
static int QuashTerminal;
struct termios Quash_tmodes;
int position;
#define null '\0'
struct Job {
    char* filename;
    int jobid;
    int pid;

};
static struct Job* jobs[10];
int job_count = 0;


void parse(char* input) {
    int args_count = 0;
    char* cur_input = dup2(input);
    command = strtok(input, " ");
    while (args[args_count] != NULL) {
        args[args_count] = strtok(NULL, " ");
        pos++;
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
    
    
}

int setPath() {
    int len;
    i = 0;
    len = 0;
    char* pathcmd = (char *)malloc(sizeof(char)*50);
    char* path = var1;
    len = strlen(var1);

    if(strncmp(path, "HOME=", 5) == 0) {
        strncpy(pathcmd, path + 5, (len - 5));
        printf("Home is set to: %s \n", pathcmd);
        if((setenv("HOME",pathcmd,1)) ==- 1){
            printf("Home was not correctly set.\n");
        }
    } else if(strncmp(path, "PATH=", 5) == 0) {
        strncpy(pathcmd, path + 5, (len - 5));
        printf("Path is set to: %s \n", pathcmd);
        if((setenv("PATH",pathcmd,1)) ==- 1){
            printf("Path was not correctly set.\n");
        }
    } else {
        printf("set command was not understood.\n");
    }

    return 1;

}

void cd(const char* p) {
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
    else {
        //just to test cd and path stuff, not good just uses sh.
        system(inputString);
    }
    /*
    //if (strcmp("ls", command) ==0) {
    //ls();??
    //}

    else {
    //????
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
    */
}

void quashPipe(char* cmd1[], char* cmd2[]) {
    int spipe[2];
    pipe(spipe);
    pid_t pid;
    pid = fork();

    if (pid == 0) {
        dup2(spipe[1], STDOUT_FILENO);
        close(spipe[0]);
        close(spipe[1]);
        if (execvp(*cmd1, cmd1) == -1) {
            perror("No good, sir.");
        }
        exit(0);
    } else {
        dup2(spipe[0], STDIN_FILENO);
        close(spipe[0]);
        close(spipe[1]);
        waitpid(pid, &status, 0);
        if (execvp(*cmd2, cmd2) == -1) {
            perror("Something went wrong...");
        }
    }
}

void launchJob(char* cmd[], char* filename) {

}

char* trimwhitespace(char *str)
{
    char* end;
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
    rl_bind_key('\t', rl_complete);
    //Pretty intro
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
    char* quash_prompt[100];
    char* input_string;
    struct Job *job;
    jobs[job_count];
    job_count++;
    do {
        snprintf(quash_prompt, sizeof(quash_prompt),"%s:%s:$ ", getenv("USER"), getcwd(NULL, 1024));
        input_string = readline(quash_prompt);
        if (input_string) {
            add_history(input_string);
        }
        parse(input_string);
        
    } while (flag);


    return(0);
}
