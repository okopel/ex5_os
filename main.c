#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>


char getch();

int main() {
    printf("start");
    int filedes[2];


    pipe(filedes);
    pid_t pid;
    pid = fork();
    if (pid > 0) {//father
        char c;
        do {
            c = getch();
            write(filedes[1], c, sizeof(c));
            printf("c: %c\n", c);
        } while (c != 0);
    } else { //child
        char d;
        do {
            read(filedes[0], d, sizeof(d));
        } while (d != 0);
    }
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