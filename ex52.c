
/************
 * Ori Kopel
 * 205533151
 * ex3_os
 ************/
#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>

#define B_SIZE 20
char board[B_SIZE][B_SIZE];
int place = B_SIZE / 2;
int placeH = B_SIZE;
bool up = false;

void down();

void initionBoard();

void autoPlay();

void erasePrevStep();

void printBoard();


void left();

void right();

void rotate();

void userPlay();


//##########################################
void userPlay() {
    erasePrevStep();
    char c = getchar();
    switch (c) {
        case 'a':
            printf("a");
            left();
            break;
        case 'd':
            printf("d");
            right();
            break;
        case 's':
            printf("s");
            down();
            break;
        case 'w':
            printf("w");
            rotate();
            break;
        case 'q':
            printf("q");
            exit(1);
        default:
            //the last step
            printBoard();
            break;
    }

}

void initionBoard() {
    for (int i = 0; i < B_SIZE; i++) {
        board[i][0] = '*';
        board[i][B_SIZE - 1] = '*';
        board[B_SIZE - 1][i] = '*';
        for (int j = 1; j < B_SIZE - 1; j++) {
            if (i == B_SIZE - 1) {
                break;
            }
            board[i][j] = ' ';
        }
    }
}

void autoPlay() {
    signal(SIGUSR2, autoPlay);
    alarm(1);
    erasePrevStep();
    down();
}

void erasePrevStep() {
    board[placeH][place] = ' ';
    if (up) {
        board[placeH + 1][place] = ' ';
        board[placeH - 1][place] = ' ';
    } else {
        board[placeH][place + 1] = ' ';
        board[placeH][place - 1] = ' ';
    }
}

void printBoard() {
    system("clear");
    board[placeH][place] = '-';
    if (up) {
        board[placeH + 1][place] = '-';
        board[placeH - 1][place] = '-';
    } else {
        board[placeH][place + 1] = '-';
        board[placeH][place - 1] = '-';
    }
    for (int i = 0; i < B_SIZE; i++) {
        for (int j = 0; j < B_SIZE; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}


void left() {
    if ((place < 2 && !up) || (place < 1 && up)) {
        return;
    }
    place--;
    printBoard();
}

void right() {
    if ((place > (B_SIZE - 1) && !up) || (place > B_SIZE && up)) {
        return;
    }
    place++;
    printBoard();

}

void down() {
    if ((placeH == 0 && !up) || (placeH == 1 && up)) {
        placeH = B_SIZE;
    } else {
        placeH--;
    }
    printBoard();

}

void rotate() {
    if ((placeH == 1 && !up) || ((place == 0 || place == B_SIZE - 1) && up)) {
        return;
    }
    up = !up;
    printBoard();
}

void child() {
    pause();
    child();
}

void main() {
    initionBoard();
    signal(SIGUSR2, userPlay);
    signal(SIGALRM, autoPlay);
    alarm(1);
    while (true) {
        pause();
    }
}