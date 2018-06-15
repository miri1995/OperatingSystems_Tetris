

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define STDERR 2
#define ERROR "Error in system call\n"
#define SIZE 20
#define LAY_STATE 0
#define STAND_STATE 1




typedef struct Board {
    char board[SIZE][SIZE];
    int x;
    int y;
    int state; //1 if change 0 regular
} Board;

Board board;

void fail() {
    write(STDERR, ERROR, sizeof(ERROR)-1);
    exit(-1);
}


void printBoard(Board* board) {
      if (system("clear")==-1) {
         fail();
      }

  int i;

    //print the board
    for (i = 0; i <SIZE ; ++i) {
        int j;
        for (j = 0; j < SIZE; ++j) {
            printf("%c", board->board[i][j]);
        }
        printf("\n");
    }
}


void initializationShape(Board* board){

    board->state=0;
    board->x= SIZE/2;
    board->y= 0;

    int i,j;
    for (i = 0; i <SIZE ; ++i) {

        for (j = 0; j < SIZE; ++j) {
            board->board[i][j] = ' ';
        }
    }
    //initialize the board and player
    for (i = 0; i <SIZE ; ++i) {

        for (j = 0; j < SIZE; ++j) {
            if(i==SIZE-1||j==0||j==SIZE-1){
                board->board[i][j]='*';
            }
            if (j==board->x&&i==board->y) {
                board->board[i][j] = '-';
                if (board->state==LAY_STATE) {
                    board->board[i][j-1] ='-';
                    board->board[i][j+1]='-';
                } else if (board->state==STAND_STATE) {
                    board->board[i-1][j] = '-';
                    board->board[i+1][j] = '-';
                }
            }

        }
    }


}

void moveLeft(Board* board){
    if(board->x < 3){
        initializationShape(board);
        return;
    }
    board->x-=1;



}

void moveRight(Board* board){

    if(board->x>=SIZE-3){
        initializationShape(board);
        return;
    }
    board->x+=1;



}

void moveDown(Board* board){
    if(board->state==LAY_STATE) {
        if (board->y >= SIZE - 2) {
            initializationShape(board);
            return;
        }
        board->y += 1;

        board->board[board->y][board->x] = '-';
        board->board[board->y][board->x - 1] = '-';
        board->board[board->y][board->x + 1] = '-';
    }else{
        if (board->y >= SIZE - 3) {
            initializationShape(board);
            return;
        }
        board->y += 1;

        board->board[board->y][board->x] = '-';
        board->board[board->y-1][board->x] = '-';
        board->board[board->y+1][board->x] = '-';
    }

}

void changeState(Board* board){
    if(board->y>=SIZE-2){
        initializationShape(board);
        return;
    }
    if(board->state==LAY_STATE){

        board->board[board->y][board->x]='-';
        board->board[board->y-1][board->x]='-';
        board->board[board->y+1][board->x]='-';
        board->board[board->y][board->x-1]=' ';
        board->board[board->y][board->x+1]=' ';
        board->state=1;
    }else if(board->state==STAND_STATE){
        board->board[board->y][board->x]='-';
        board->board[board->y][board->x-1]='-';
        board->board[board->y][board->x+1]='-';
        board->board[board->y-1][board->x]=' ';
        board->board[board->y+1][board->x]=' ';
        board->state=0;
    }
}

void moveSteps(int sig){

    signal(SIGUSR2, moveSteps);

    char step;
    step = (char)getchar();
    switch (step){
        case 'a':
            moveLeft(&board);
            break;
        case 'd':
            moveRight(&board);
            break;
        case 's':
            moveDown(&board);
            break;
        case 'w':
            changeState(&board);
            break;
        case 'q':
            exit(-1);
        default:
            return;

    }
    printBoard(&board);
}


void alarmFunc (int sig){
    signal(SIGALRM,alarmFunc);
    alarm(1);
    int i,j;
    for (i = 0; i <SIZE ; ++i) {

        for (j = 0; j < SIZE; ++j) {
            board.board[i][j] = ' ';
        }
    }
    //initialize the board
    for (i = 0; i <SIZE ; ++i) {

        for (j = 0; j < SIZE; ++j) {
            if (i == SIZE - 1 || j == 0 || j == SIZE - 1) {
                board.board[i][j] = '*';
            }
        }
    }
    moveDown(&board);
    printBoard(&board);

}

void start(Board *board) {

    initializationShape(board);
    printBoard(board);
    signal(SIGALRM, alarmFunc);
    alarm(1);
    signal(SIGUSR2, moveSteps);
    while (1) {
        //wait for signals
        pause();
    }

}

int main(){

    start(&board);
    return 0;
}




