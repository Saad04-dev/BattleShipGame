#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5
#define EMPTY '-'
#define SHIP 'S'
#define HIT 'X'
#define MISS 'O'

void setupBoard(char board[SIZE][SIZE]);
void printBoard(char board[SIZE][SIZE],int hideShips);
void placeShips(char board[SIZE][SIZE],int numShips);
int isMoveValid(char board[SIZE][SIZE],int row,int col);
int makeMove(char board[SIZE][SIZE],int row,int col);
void botEasy(char board[SIZE][SIZE],int *row,int *col);
void botMedium(char board[SIZE][SIZE],int *row,int *col);
void botHard(char board[SIZE][SIZE],int *row,int *col);
int gameFinished(char board[SIZE][SIZE]);

void setupBoard(char board[SIZE][SIZE]){
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++) {
            board[i][j]=EMPTY;
        }
    }
}

//when displaying the boardwith hidden cells
void printBoard(char board[SIZE][SIZE],int hideShips){
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            if(hideShips && board[i][j]==SHIP){
                printf("%c ",EMPTY);
            } else{
                printf("%c ",board[i][j]);
            }
        }
        printf("\n");
    }
}

//funcolumnstion to put the ships randomely on the board 
void placeShips(char board[SIZE][SIZE],int numShips){
    int count=0;
    while(count<numShips){
        int row=rand() % SIZE;
        int col=rand() % SIZE;
        if(board[row][col]==EMPTY){
            board[row][col]=SHIP;
            count++;
        }
    }
}

//validate the move
int isMoveValid(char board[SIZE][SIZE],int row,int col){
    return row>=0 && row<SIZE && col>=0 && col<SIZE &&
           (board[row][col]==EMPTY || board[row][col]==SHIP);
}

//return 1 if its a hit 0 if its a miss
int makeMove(char board[SIZE][SIZE],int row,int col){
    if(board[row][col]==SHIP){
        board[row][col]=HIT;
        return 1;
    } else{
        board[row][col]=MISS;
        return 0;
    }
}

//THE EASY BOT:random moves
void botEasy(char board[SIZE][SIZE],int *row,int *col){
    while(1){
        *row=rand() % SIZE;
        *col=rand() % SIZE;
        if(isMoveValid(board,*row,*col)){
            break;
        }
    }
}

//THE MEDIUM BOT:target adjacent cells after a hit
void botMedium(char board[SIZE][SIZE],int *row,int *col){
    for(int r=0;r<SIZE;r++){
        for(int c=0;c<SIZE;c++){
            if(board[r][c]==HIT){
    //now we search for the adjacent cells and hit them
           int directions[4][2]={{-1,0},{1,0},{0, -1},{0, 1}};
                for(int d=0;d<4;d++) {
                    int nrows=r+directions[d][0];
                    int ncolumns=c+directions[d][1];
                    if(isMoveValid(board,nrows,ncolumns)){
                        *row=nrows;
                        *col=ncolumns;
                        return;
                    }
                }
            }
        }
    }
    //random move
    botEasy(board,row,col);
}

//THE HARD BOT:new advancolumnsed targeting strategy
void botHard(char board[SIZE][SIZE],int *row,int *col){
    botMedium(board,row,col);
}

//to check if the game is over
int gameFinished(char board[SIZE][SIZE]){
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            if(board[i][j]==SHIP){
                return 0;
            }
        }
    }
    return 1;
}

//main funcolumnstion
void play_battleship(){
    char humanGrid[SIZE][SIZE],botGrid[SIZE][SIZE];
    int playerTurn=1; // 1=player/ 0=bot
    srand(time(NULL));

    setupBoard(humanGrid);
    setupBoard(botGrid);
    placeShips(humanGrid, 3);
    placeShips(botGrid, 3);

    printf("Your board:\n");
    printBoard(humanGrid, 0);

    char difficulty;
    printf("Select bot difficulty (e: Easy, m: Medium, h: Hard): ");
    scanf(" %c",&difficulty);

    while(1){
        int row,col;

        if(playerTurn){
            printf("Your turn! Enter row and column: ");
            scanf("%d %d",&row,&col);
            if(!isMoveValid(botGrid, row, col)){
                printf("Invalid move, try again.\n");
                continue;
            }
            if(makeMove(botGrid, row, col)){
                printf("You hit a ship!\n");
            } else{
                printf("You missed.\n");
            }
        } else{
            printf("Bot's turn...\n");
            if(difficulty=='e'){
                botEasy(humanGrid,&row,&col);
            } else if(difficulty=='m'){
                botMedium(humanGrid,&row,&col);
            } else{
                botHard(humanGrid,&row,&col);
            }
            printf("Bot attacks (%d, %d).\n",row,col);
            if(makeMove(humanGrid,row,col)){
                printf("The bot hit your ship!\n");
            } else{
                printf("The bot missed.\n");
            }
        }

        printf("Bot's Board:\n");
        printBoard(botGrid,1);
        printf("Your Board:\n");
        printBoard(humanGrid,0);

        if(gameFinished(botGrid)){
            printf("You win!\n");
            break;
        } else if(gameFinished(humanGrid)){
            printf("The bot wins!\n");
            break;
        }

        playerTurn=!playerTurn;
    }
}

int main(){
    play_battleship();
    return 0;
}