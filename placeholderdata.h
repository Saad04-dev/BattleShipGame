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
    char name[20];               // Name of the ship
    char id;                     // Ship identifier
    int size;                    // Size of the ship
    int occupiedCells[GRID_SIZE][3]; // Coordinates of occupied cells (row, col, hit status)
} Ship;

// Player structure
typedef struct {
    char name[20];               // Player's name
    int turn;                    // Indicates if it's this player's turn
    int numOfShipsSunken;        // Number of sunken ships
    int numOfArtillery;          // Artillery unlocked status
    int numOfRadars;             // Radar uses remaining
    int numOfSmokeScreensPerformed; // Number of smoke screens used
    int numOfTorpedo;            // Torpedo unlocked status
    char grid[GRID_SIZE][GRID_SIZE];  // Player's own grid
    char board[GRID_SIZE][GRID_SIZE]; // Board tracking opponent's moves
    char hits[GRID_SIZE][GRID_SIZE];  // Tracking hits and misses
    Ship ships[TOTALNUMBEROFSHIPS];   // Player's ships
} Player;

// Global variables
extern char affectedArea[GRID_SIZE][GRID_SIZE];
extern char game_difficulty;

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

// Functions for hit or miss mechanics
void HitOrMiss(Player *attacker, Player *defender, int x, int y, char movetype, char orientation);
void HitOrMissMessageDisplay(int movesuccess);
void markAffectedArea(int x, int y, char moveType, char orientation);
int isShipSunk(Ship *ship);

// Validation functions
int validateCoordinates(int x, int y);
int fire(Player *opponent, int x, int y);
void radar(Player *opponent, int x, int y);
void smoke_screen(char board[GRID_SIZE][GRID_SIZE], int x, int y);
void artillery(Player *opponent, int x, int y);
void torpedo(Player *opponent, int index, char axis);
int validateSpecialMoveUsage(int *uses, const char *move);

#endif // PLACEHOLDERDATA_H
