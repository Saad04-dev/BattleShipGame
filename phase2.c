#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SIZE 10
#define EMPTY '-'
#define SHIP 'S'
#define HIT '*'
#define MISS 'O'

typedef struct {
    int x, y;
} Shot;

// Function prototypes
void setupBoard(char board[SIZE][SIZE]);
void printBoard(char board[SIZE][SIZE], int hideShips);
void placeShipsManual(char board[SIZE][SIZE]);
void placeShipsHard(char board[SIZE][SIZE]);
int isMoveValid(char board[SIZE][SIZE], int row, int col);
int makeMove(char board[SIZE][SIZE], int row, int col);
Shot getShotEasy();
int gameFinished(char board[SIZE][SIZE]);
void placeShip(char board[SIZE][SIZE], int x, int y, int size, int horizontal);
int canPlaceShip(char board[SIZE][SIZE], int x, int y, int size, int horizontal);

// Setup an empty board
void setupBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

// Print the board with optional hidden ships
void printBoard(char board[SIZE][SIZE], int hideShips) {
    printf("  ");
    for (int col = 1; col <= SIZE; col++) {
        printf("%d ", col);
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%c ", 'A' + i);
        for (int j = 0; j < SIZE; j++) {
            if (hideShips && board[i][j] == SHIP) {
                printf("%c ", EMPTY);
            } else {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// Place ships manually
void placeShipsManual(char board[SIZE][SIZE]) {
    int shipSizes[] = {5, 4, 3, 2};
    int numShips = 4;

    printf("Place your ships on the board.\n");
    printBoard(board, 0);

    for (int i = 0; i < numShips; i++) {
        int size = shipSizes[i];
        int x, y, horizontal, placed = 0;

        while (!placed) {
            printf("Place ship of size %d (enter row [A-J], column [1-10], and orientation [0: vertical, 1: horizontal]): ", size);
            char rowChar;
            scanf(" %c %d %d", &rowChar, &x, &horizontal);
            rowChar = toupper(rowChar); // Ensure row is uppercase
            y = rowChar - 'A';          // Convert row letter to index
            x -= 1;                     // Convert column number to index

            if (canPlaceShip(board, x, y, size, horizontal)) {
                placeShip(board, x, y, size, horizontal);
                placed = 1;
                printf("Ship placed successfully.\n");
                printBoard(board, 0);
            } else {
                printf("Invalid placement. Try again.\n");
            }
        }
    }
}

// Randomly place ships for hard difficulty
void placeShipsHard(char board[SIZE][SIZE]) {
    int shipSizes[] = {5, 4, 3, 3, 2};
    for (int i = 0; i < 5; i++) {
        int placed = 0;
        while (!placed) {
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            int horizontal = rand() % 2;
            if (canPlaceShip(board, x, y, shipSizes[i], horizontal)) {
                placeShip(board, x, y, shipSizes[i], horizontal);
                placed = 1;
            }
        }
    }
}

// Check if a move is valid
int isMoveValid(char board[SIZE][SIZE], int row, int col) {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE &&
           (board[row][col] == EMPTY || board[row][col] == SHIP);
}

// Make a move
int makeMove(char board[SIZE][SIZE], int row, int col) {
    if (board[row][col] == SHIP) {
        board[row][col] = HIT;
        return 1;
    } else {
        board[row][col] = MISS;
        return 0;
    }
}

// Check if the game is finished
int gameFinished(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == SHIP) {
                return 0;
            }
        }
    }
    return 1;
}

// Helper to place a ship
void placeShip(char board[SIZE][SIZE], int x, int y, int size, int horizontal) {
    for (int i = 0; i < size; i++) {
        if (horizontal) {
            board[y][x + i] = SHIP;
        } else {
            board[y + i][x] = SHIP;
        }
    }
}

// Check if a ship can be placed
int canPlaceShip(char board[SIZE][SIZE], int x, int y, int size, int horizontal) {
    for (int i = 0; i < size; i++) {
        if (horizontal) {
            if (x + i >= SIZE || board[y][x + i] != EMPTY) {
                return 0;
            }
        } else {
            if (y + i >= SIZE || board[y + i][x] != EMPTY) {
                return 0;
            }
        }
    }
    return 1;
}

// Easy bot logic
Shot getShotEasy() {
    Shot shot;
    shot.x = rand() % SIZE;
    shot.y = rand() % SIZE;
    return shot;
}

// Main game loop
void play_battleship() {
    char humanGrid[SIZE][SIZE], botGrid[SIZE][SIZE];
    int playerTurn = 1; // 1 = player, 0 = bot
    srand(time(NULL));

    setupBoard(humanGrid);
    setupBoard(botGrid);

    placeShipsManual(humanGrid);
    placeShipsHard(botGrid);

    printf("Your board:\n");
    printBoard(humanGrid, 0);

    while (1) {
        char rowChar;
        int col, row;
        Shot botShot;

        if (playerTurn) {
            printf("Your turn! Enter row (A-J) and column (1-10): ");
            scanf(" %c %d", &rowChar, &col);
            rowChar = toupper(rowChar);
            row = rowChar - 'A';
            col -= 1;

            if (!isMoveValid(botGrid, row, col)) {
                printf("Invalid move, try again.\n");
                continue;
            }

            if (makeMove(botGrid, row, col)) {
                printf("You hit a ship!\n");
            } else {
                printf("You missed.\n");
            }
        } else {
            botShot = getShotEasy();
            printf("Bot attacks (%c, %d).\n", 'A' + botShot.y, botShot.x + 1);

            if (makeMove(humanGrid, botShot.y, botShot.x)) {
                printf("The bot hit your ship!\n");
            } else {
                printf("The bot missed.\n");
            }
        }

        printf("\nBot's Board:\n");
        printBoard(botGrid, 1);
        printf("\nYour Board:\n");
        printBoard(humanGrid, 0);

        if (gameFinished(botGrid)) {
            printf("You win!\n");
            break;
        } else if (gameFinished(humanGrid)) {
            printf("The bot wins!\n");
            break;
        }

        playerTurn = !playerTurn;
    }
}

int main() {
    play_battleship();
    return 0;
}
