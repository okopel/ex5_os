/************
 * Ori Kopel
 * 205533151
 * ex3_os
 ************/
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>


char getch();

int flag = 0;

void mySig() {
    printf("signal has sent\n");
    signal(SIGUSR2, mySig);
}

void child() {
    pause();
    printf("INTERAPT!!");
    child();
    /*char d[100];
    do {
        read(pipeReader, d, sizeof(d));
        sleep(1);
    } while (d[0] != 'q');*/

}

void father(int status, int pipeWriter) {
    char c;
    char buf[1];
    do {
        c = getch();
        if (c == 'a' || c == 's' || c == 'd' || c == 'w' || c == 'q') {
            buf[0] = c;
            write(pipeWriter, buf, sizeof(buf));
            kill(status, SIGUSR2);
            printf("c: %c\n", c);
        }
    } while (c != 'q');
}

int main() {
    int fd[2];
    pipe(fd);
    pid_t status;
    signal(SIGUSR2, mySig);

    status = fork();

    if (status == 0) {//child
        signal(SIGUSR2, mySig);
        child();
        //exit(1);
    } else { //father
        father(status, fd[1]);
    }
    //close(filedes[0]);//todo close from father?
    //close(filedes[1]);
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