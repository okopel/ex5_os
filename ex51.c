/************
 * Ori Kopel
 * 205533151
 * ex3_os
 ************/
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#define STDERR 2
#define ERR_MSG "Error in system call\n"

char getch();

void printError();

void father(int pid, int writer);


int main() {
    int fd[2];
    pipe(fd);
    pid_t status = fork();
    if (status < 0) {
        printError();
    } else if (status == 0) {//child
        dup2(fd[0], 0);
        execlp("./draw.out", "./draw.out", NULL);
        printError();
    } else { //father
        father(status, fd[1]);

    }

//close(fd[0]);//todo close from father?
//close(fd[1]);
}

void printError() {
    write(STDERR, ERR_MSG, strlen(ERR_MSG));
}

void father(int status, int pipeWriter) {
    char c;
    char buf[1];
    do {
        c = getch();
        if (c == 'a' || c == 's' || c == 'd' || c == 'w' || c == 'q') {
            buf[0] = c;
            if (write(pipeWriter, &buf, sizeof(buf)) < 0) {
                printError();
            }
            kill(status, SIGUSR2);
            printf("c: %c\n", c);
        }
    } while (c != 'q');
}

/**
 * Get char from the user directly without Enter needed.
 * @return a char.
 * NOTE: this function works just in the Terminal
 */
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
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}