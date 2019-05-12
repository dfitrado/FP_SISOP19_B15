#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/wait.h>
struct arg {
    char minute, hour, day, month, week;
    char job[2000], program[2000];
};

void* runProgram();
void Jobs();
int isConfigChanged(const char *path, time_t *oldMTime);
void killAllThread();

int totalThread = 0;
pthread_t *thread;

int main() {
    pid_t pid, sid;

    pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    umask(0);
    sid = setsid();
    if (sid < 0) exit(EXIT_FAILURE);

    if ((chdir("/")) < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    time_t oldCOnfigMTime = (time_t) 0;

    Jobs();

    while(1) {
        if((isConfigChanged("/home/wildangbudhi/Documents/FP_SISOP19_B15/config.txt", &oldCOnfigMTime))){
            killAllThread();
            Jobs();
        }
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}

void killAllThread(){
    int i;
    for(i=0; i<totalThread; i++)
        pthread_cancel(thread[i]);
}

int isConfigChanged(const char *path, time_t *oldMTime){
    struct stat file_stat;
    stat(path, &file_stat);
    int res = (file_stat.st_mtime > *oldMTime);
    if(res) *oldMTime = file_stat.st_mtime;
    return res;
}

void Jobs(){
    FILE *config = fopen("/home/wildangbudhi/Documents/FP_SISOP19_B15/config.txt", "r");
    char in[1000];
    int i = 0;

    while(fgets(in, 1000, config)){
        thread = realloc(thread, sizeof(pthread_t) * (i+1));
        struct arg *a = (struct arg*) malloc(sizeof(struct arg));
        sscanf(in, "%c %c %c %c %c %s %s", &a->minute, &a->hour, &a->day, &a->month, &a->week, a->job, a->program);
        pthread_create( &thread[i], NULL, runProgram, (void*) a);
        i++;
    }

    totalThread = i;

    fclose(config);
}

void* runProgram( void *ptr ){

    while(1){
        time_t t = time(NULL);
        struct tm temp = *localtime(&t);
        if((((struct arg*)ptr)->minute - '0') == temp.tm_min || ((struct arg*)ptr)->minute == '*')
            if((((struct arg*)ptr)->hour - '0') == temp.tm_hour || ((struct arg*)ptr)->hour == '*')
                if((((struct arg*)ptr)->day - '0') == temp.tm_mday || ((struct arg*)ptr)->day == '*')
                    if((((struct arg*)ptr)->month - '0') == (temp.tm_mon + 1) || ((struct arg*)ptr)->month == '*')
                        if((((struct arg*)ptr)->week - '0') == temp.tm_wday || ((struct arg*)ptr)->week == '*'){
                            int child = fork();
                            int status;
                            if(child == 0){
                                char *arg[] = {((struct arg*)ptr)->job, ((struct arg*)ptr)->program, NULL};
                                execv(((struct arg*)ptr)->job, arg);
                            }
                            else{
                                while(wait(&status) > 0);
                            }
                        }
        sleep(1);
    }

}
