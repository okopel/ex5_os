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


/**
 * initialization of the Tetris board.
 */
void initializeBoard();

/**
 * erase the last shape before write the new one.
 */
void erasePrevStep();

/**
 * draw the shape in the new place.
 */
void paintShape();

/**
 * move the shape a line down.
 */
void autoPlay();

/**
 * move the shape by user key.
 */
void userPlay();

/**
 * print the board and the shape to the screen.
 */
void printBoard();

/**
 * move left the shape.
 */
void left();

/**
 * move right the shape.
 */
void right();

/**
 * move down the shape.
 */
void down();

/**
 * rotate the shape.
 */
void rotate();


int main() {
    initializeBoard();
    signal(SIGUSR2, userPlay);
    signal(SIGALRM, autoPlay);
    alarm(1);
    while (true) {
        pause();
    }
    return 1;
}

void userPlay() {
    //the char get from stdin of pipe, from ex51.c
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
            //print again the last step because another key.
            printBoard();
            break;
    }
}

void initializeBoard() {
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
    //reset the signal dest
    signal(SIGALRM, autoPlay);
    //timer to the next step
    alarm(1);
    //the step every timer
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

void paintShape() {
    board[placeH][place] = '-';
    if (up) {
        board[placeH + 1][place] = '-';
        board[placeH - 1][place] = '-';
    } else {
        board[placeH][place + 1] = '-';
        board[placeH][place - 1] = '-';
    }
}

void printBoard() {
    system("clear");
    paintShape();
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
    if ((place > (B_SIZE - 4) && !up) || (place > (B_SIZE - 3) && up)) {
        return;
    }
    erasePrevStep();
    place++;
    printBoard();

}

void down() {
    erasePrevStep();
    if ((placeH >= (B_SIZE - 2) && !up) || (placeH >= (B_SIZE - 3) && up)) {
        placeH = 0;
    } else {
        placeH++;
    }
    printBoard();

}

void rotate() {
    if ((placeH >= (B_SIZE - 2) && !up) || ((place <= 1 || place >= B_SIZE - 2) && up)) {
        return;
    }
    erasePrevStep();
    up = !up;
    printBoard();
}


