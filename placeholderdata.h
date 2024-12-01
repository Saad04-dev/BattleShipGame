#ifndef PLACEHOLDERDATA_H
#define PLACEHOLDERDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define GRID_SIZE 10
#define TOTALNUMBEROFSHIPS 4
#define CARRIER_SIZE 5
#define BATTLESHIP_SIZE 4
#define DESTROYER_SIZE 3
#define SUBMARINE_SIZE 2

// Ship structure
typedef struct {
    char name[20];           // Name of the ship
    int size;                // Size of the ship
    int occupiedCells[10][10]; // Coordinates of occupied cells
} Ship;

// Player structure
typedef struct {
    char name[20];
    int turn;
    int numOfShipsSunken;
    int numOfArtillery;
    int numOfRadars;
    int numOfTorpedo;
    int numOfSmokeScreensPerformed;
    char grid[GRID_SIZE][GRID_SIZE];  
    Ship ships[TOTALNUMBEROFSHIPS]; // Player's ships
    char board[GRID_SIZE][GRID_SIZE]; // Player's board
    char hits[GRID_SIZE][GRID_SIZE];  // Board for tracking hits/misses
    int ships_remaining; // Remaining ships
} Player;

// Function prototypes
void displayAvailableMoves(Player *currentPlayer);
char set_game_difficulty();
char get_game_difficulty();
void initialize_player(Player *player);
void initialize_board(char board[GRID_SIZE][GRID_SIZE]);
void display_opponent_grid(char board[GRID_SIZE][GRID_SIZE], char game_difficulty);
int column_to_index(char column);
int can_place_ship(char board[GRID_SIZE][GRID_SIZE], int row, int col, int size, char orientation);
void place_ship(char board[GRID_SIZE][GRID_SIZE], int row, int col, int size, char orientation, char shipID);
void clear_screen();
void place_ships_for_player(char board[GRID_SIZE][GRID_SIZE], Player *player);
void player_turn(char opponent_board[GRID_SIZE][GRID_SIZE], Player *player);
int check_win(char board[GRID_SIZE][GRID_SIZE]);
int isGameOver(Player *player1, Player *player2);
void endGame(Player *winner);
void gameLoop(Player *player1, Player *player2);

// Additional function prototypes from main function
int validateCoordinates(int x, int y); // Validation function for coordinates
int fire(Player *opponent, int x, int y); // Fire at given coordinates
void radar(Player *opponent, int x, int y); // Radar sweep at coordinates
void smoke_screen(char board[GRID_SIZE][GRID_SIZE], int x, int y); // Deploy smoke screen
void artillery(Player *opponent, int x, int y); // Artillery strike at coordinates
void torpedo(Player *, int index, char axis); // Torpedo strike
int validateSpecialMoveUsage(int *uses, const char *move); // Validate usage of special moves

#endif // PLACEHOLDERDATA_H
