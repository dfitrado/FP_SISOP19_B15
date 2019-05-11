#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pthread.h>

struct arg {
    char minute, hour, day, month, week;
    char job[2000];
};

void* runProgram();
void Jobs();
int isConfigChanged(const char *path, time_t *oldMTime);

static int confChange = 0;
pthread_t thread;

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
    while(1) {
        if((isConfigChanged("/home/wildangbudhi/Documents/FP_SISOP19_B15/config.txt", &oldCOnfigMTime))){
            pthread_cancel(thread);
            Jobs();
        }
        sleep(1);
    }

    exit(EXIT_SUCCESS);
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

    while(fgets(in, 1000, config)){
        struct arg *a = (struct arg*) malloc(sizeof(struct arg));
        sscanf(in, "%c %c %c %c %c %[^\n]", &a->minute, &a->hour, &a->day, &a->month, &a->week, a->job);
        pthread_create( &thread, NULL, runProgram, (void*) a); 
    }

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
                            system(((struct arg*)ptr)->job);
                        }
        sleep(60);
    }

}
