
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
int placeH = 0;
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
    char c = getchar();
    switch (c) {
        case 'a':
            left();
            break;
        case 'd':
            right();
            break;
        case 's':
            down();
            break;
        case 'w':
            rotate();
            break;
        case 'q':
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
    signal(SIGALRM, autoPlay);
    alarm(1);
    //erasePrevStep();
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
    if ((place < 3 && !up) || (place < 2 && up)) {
        return;
    }
    erasePrevStep();
    place--;
    printBoard();
}

void right() {
    if ((place > (B_SIZE - 2) && !up) || (place > (B_SIZE - 1) && up)) {
        return;
    }
    erasePrevStep();
    place++;
    printBoard();

}

void down() {
    if ((placeH == (B_SIZE - 1) && !up) || (placeH == (B_SIZE - 2) && up)) {
        placeH = 0;
    } else {
        erasePrevStep();
        placeH++;
        printBoard();
    }

}

void rotate() {
    if ((placeH == B_SIZE && !up) || ((place == 0 || place == B_SIZE - 1) && up)) {
        return;
    }
    erasePrevStep();
    up = !up;
    printBoard();
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