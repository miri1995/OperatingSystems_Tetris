
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <termios.h>
#include <signal.h>
#include <string.h>

#define STDERR 2
#define ERROR "Error in system call\n"



char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}



int gameKey(char str){
    switch (str){
        case 'a':
        case 'd':
        case 's':
        case 'w':
        case 'q':
            return 1;
        default:
            return 0;
    }
}


void fail() {
    write(STDERR, ERROR, sizeof(ERROR)-1);
    exit(-1);
}

void callExec(){
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) {  // son
        dup2(fd[0],0);
        char *args[] = {"./draw.out", NULL};
        int n =  execvp("./draw.out", args);
        if (n == -1) {
              fail();
        }
    }else{ //father
        char input;
        while(1){
            input=getch();
            if(!gameKey(input)){
                continue;
            }else {
                write(fd[1], &input, 1);//mybey & before input
                kill(pid, SIGUSR2);
                if (input == 'q') {
                    break;
                }
            }
        }

    }
}



int main() {
    callExec();
    return 0;
}

