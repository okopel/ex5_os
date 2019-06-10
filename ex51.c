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
#define EX52_FILE "./draw.out"


/**
 * Get char from the user directly without Enter needed.
 * @return a char.
 * NOTE: this function works just in the Terminal
 */
char getch();

/**
 * print err msg to the stdin 2
 */
void printError();

/**
 * Get char from keyboard and send them to stdin if legal.
 * @param pid id of chile in order to send signal
 * @param writer in order to write the pipe.
 */
void father(int pid, int writer);


int main() {
    //pipe des
    int fd[2];
    pipe(fd);
    pid_t status = fork();
    if (status < 0) {//Error case
        printError();
    } else if (status == 0) {//child case
        dup2(fd[0], 0);//set the writer to EX52
        execlp(EX52_FILE, EX52_FILE, NULL);
        printError();
    } else { //father case
        father(status, fd[1]);
    }
    close(fd[0]);
    close(fd[1]);
}

void printError() {
    write(STDERR, ERR_MSG, strlen(ERR_MSG));
}

void father(int pid, int writer) {
    char c;
    char buf[1];
    do {
        c = getch();
        if (c == 'a' || c == 's' || c == 'd' || c == 'w' || c == 'q') {
            buf[0] = c;
            if (write(writer, &buf, sizeof(buf)) < 0) {
                printError();
            }
            kill(pid, SIGUSR2);
        }
    } while (c != 'q');
}


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