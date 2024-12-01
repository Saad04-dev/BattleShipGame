#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "placeholderdata.h"

// Global variable for game difficulty
char game_difficulty;

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

// Functions
void displayAvailableMoves(Player *currentPlayer)
{
    printf("\nIt's %s's turn!\n", currentPlayer->name);
    printf("Available Moves:\n");
    printf("1. Fire: Attempts to hit an opponent's ship at a specified coordinate.\n");
    printf("   Command: Fire [coordinate] (e.g., Fire B3)\n");
    printf("2. Radar Sweep: Reveals if there are enemy ships in a 2x2 area.\n");
    printf("   Command: Radar [top-left coordinate] (e.g., Radar B3)\n");
    printf("   Note: Limited to 3 uses per player.\n");
    printf("3. Smoke Screen: Obscures a 2x2 area of the grid from radar sweeps.\n");
    printf("   Command: Smoke [top-left coordinate] (e.g., Smoke B3)\n");
    printf("   Note: Allowed one screen per sunken ship.\n");
    printf("4. Artillery: Targets a 2x2 area, functioning similarly to Fire.\n");
    printf("   Command: Artillery [top-left coordinate] (e.g., Artillery B3)\n");
    printf("   Note: Unlocked next turn if an opponent's ship is sunk.\n");
    printf("5. Torpedo: Targets an entire row or column.\n");
    printf("   Command: Torpedo [row/column] (e.g., Torpedo B)\n");
    printf("   Note: Unlocked next turn if a third ship is sunk.\n");
}

char set_game_difficulty()
{
    char difficulty;

    while (1)
    {
        printf("Enter game difficulty (E for Easy, H for Hard): ");
        scanf(" %c", &difficulty);
        difficulty = toupper(difficulty);

        if (difficulty == 'E' || difficulty == 'H')
        {
            break;
        }
        else
        {
            printf("Invalid input. Please enter 'E' or 'H'.\n");
        }
    }
    return difficulty;
}

char get_game_difficulty()
{
    return game_difficulty;
}

void initialize_player(Player *player)
{
    player->turn = 0;
    player->numOfShipsSunken = 0;
    player->numOfArtillery = 0;
    player->numOfRadars = 3;
    player->numOfSmokeScreensPerformed = 0;
    initialize_board(player->board);
    initialize_board(player->hits);
}

void initialize_board(char board[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            board[i][j] = '~';
        }
    }
}
void initializeGrid(Player *player) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            player->grid[i][j] = ' '; 
        }
    }
}

void display_opponent_grid(char board[GRID_SIZE][GRID_SIZE], char game_difficulty)
{
    printf("   A B C D E F G H I J\n");
    for (int i = 0; i < GRID_SIZE; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (game_difficulty == 'E')
            {
                if (board[i][j] == '*')
                {
                    printf("* ");
                }
                else if (board[i][j] == 'o')
                {
                    printf("o ");
                }
                else
                {
                    printf("~ ");
                }
            }
            else if (game_difficulty == 'H')
            {
                if (board[i][j] == '*')
                {
                    printf("* ");
                }
                else
                {
                    printf("~ ");
                }
            }
        }
        printf("\n");
    }
}
int fire(Player *opponent, int x, int y) {
    if (opponent->grid[x][y] == 'S') {  
        opponent->grid[x][y] = 'X'; 
        opponent->ships_remaining--;
        printf("Hit!\n");
        return 1;
    } else {
        opponent->grid[x][y] = 'O'; 
        printf("Miss!\n");
        return 0;
    }
}
void radar(Player *opponent, int x, int y) {
    printf("Radar scan at (%d, %d):\n", x, y);
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
                if (opponent->grid[i][j] == 'S') {
                    printf("Ship found at (%d, %d)\n", i, j);
                } else {
                    printf("No ship at (%d, %d)\n", i, j);
                }
            }
        }
    }
}
void smoke(Player *player, int x, int y) {
    printf("Smoke deployed at (%d, %d):\n", x, y);
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
                player->grid[i][j] = 'S'; 
            }
        }
    }
}
void artillery(Player *opponent, int x, int y) {
    printf("Artillery strike at (%d, %d):\n", x, y);
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
                fire(opponent, i, j); 
            }
        }
    }
}
void torpedo(Player *opponent, int row_or_col, char type) {
    printf("Torpedo strike on %s %d:\n", type == 'r' ? "row" : "column", row_or_col);
    
    if (type == 'r') {
        //attack entire row
        for (int j = 0; j < GRID_SIZE; j++) {
            fire(opponent, row_or_col, j); 
        }
    } else if (type == 'c') {
        //attack entire column
        for (int i = 0; i < GRID_SIZE; i++) {
            fire(opponent, i, row_or_col); 
        }
    }
}
int column_to_index(char column)
{
    return toupper(column) - 'A';
}

int can_place_ship(char board[GRID_SIZE][GRID_SIZE], int row, int col, int size, char orientation)
{
    if (orientation == 'H')
    {
        if (col + size > GRID_SIZE)
            return 0;
        for (int i = 0; i < size; i++)
        {
            if (board[row][col + i] != '~')
                return 0;
        }
    }
    else if (orientation == 'V')
    {
        if (row + size > GRID_SIZE)
            return 0;
        for (int i = 0; i < size; i++)
        {
            if (board[row + i][col] != '~')
                return 0;
        }
    }
    return 1;
}

void place_ship(char board[GRID_SIZE][GRID_SIZE], int row, int col, int size, char orientation, char shipID)
{
    if (orientation == 'H')
    {
        for (int i = 0; i < size; i++)
        {
            board[row][col + i] = shipID;
        }
    }
    else if (orientation == 'V')
    {
        for (int i = 0; i < size; i++)
        {
            board[row + i][col] = shipID;
        }
    }
}

void playerswitch(Player *player1, Player *player2, char game_difficulty) {
    if (player1->turn == 1) { // Player one's turn
        display_opponent_grid(player2->hits, game_difficulty);
    } else if (player2->turn == 1) { // Player two's turn
        display_opponent_grid(player1->hits, game_difficulty);
    }
}

void HitOrMiss(Player *attacker, Player *defender, int x, int y, char moveType, char orientation) {
    markAffectedArea(x, y, moveType, orientation);
    int HitRegister = 0;

    // Iterate over the affected area to check for hits
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (affectedArea[i][j] == 'X') { // Affected
                char shipID = defender->grid[i][j]; // Get the ship identifier
                if (shipID != '~') { // Check if there's a ship
                    if (defender->hits[i][j] != '*') { // Ensure the cell wasn't already hit
                        for (int k = 0; k < TOTALNUMBEROFSHIPS; k++) {
                            if (defender->ships[k].id == shipID) {
                                for (int m = 0; m < defender->ships[k].size; m++) {
                                    if (defender->ships[k].occupiedCells[m][0] == i &&
                                        defender->ships[k].occupiedCells[m][1] == j) {
                                        defender->ships[k].occupiedCells[m][2] = 1; // Mark as hit
                                        break;
                                    }
                                }
                                defender->hits[i][j] = '*';
                                HitRegister++;
                                HitOrMissMessageDisplay(1);

                                if (isShipSunk(&defender->ships[k])) {
                                    printf("%s has been sunk!\n", defender->ships[k].name);
                                    defender->numOfShipsSunken++;
                                    attacker->numOfArtillery = 1;

                                    if (defender->numOfShipsSunken == 3) { // Third ship sunken
                                        attacker->numOfTorpedo = 1;
                                    }
                                }
                                break; // Exit loop after finding the correct ship
                            }
                        }
                    }
                } else {
                    if (defender->hits[i][j] == '~') { // Mark as miss
                        defender->hits[i][j] = 'o';
                    }
                }
            }
        }
    }
    if (HitRegister == 0) {
        HitOrMissMessageDisplay(0);
    }
}

int validateSpecialMoveUsage(int *remaining_uses, const char *move_name) {
    if (*remaining_uses > 0) {
        (*remaining_uses)--; 
        return 1;  
    } else {
        printf("%s move limit reached!\n", move_name);
        return 0;  
    }
}
void HitOrMissMessageDisplay(int movesuccess) {
    if (movesuccess == 0) { // MISS
        printf("Miss!\n");
    } else if (movesuccess == 1) { // HIT
        printf("Hit!\n");
    }
}
void markAffectedArea(int x, int y, char moveType, char orientation) {
    // Reset affected grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            affectedArea[i][j] = '~';
        }
    }

    // Mark affected area based on move type
    if (moveType == 'F') { // Single cell
        affectedArea[x][y] = 'X';
    } else if (moveType == 'A') { // 2x2 area
        affectedArea[x][y] = 'X';
        if (x + 1 < GRID_SIZE) affectedArea[x + 1][y] = 'X';
        if (y + 1 < GRID_SIZE) affectedArea[x][y + 1] = 'X';
        if (x + 1 < GRID_SIZE && y + 1 < GRID_SIZE) affectedArea[x + 1][y + 1] = 'X';
    } else if (moveType == 'T') {
        if (orientation == 'H') { // Row move
            for (int i = 0; i < GRID_SIZE; i++) {
                affectedArea[x][i] = 'X';
            }
        } else if (orientation == 'V') { // Column move
            for (int i = 0; i < GRID_SIZE; i++) {
                affectedArea[i][y] = 'X';
            }
        }
    }
}

int isShipSunk(Ship *ship) {
    for (int i = 0; i < ship->size; i++) {
        if (ship->occupiedCells[i][2] == 0) {
            return 0; // Not all parts are hit
        }
    }
    return 1; // All parts are hit
}

// Unlock advanced moves function
void unlockAdvancedMoves(int successful_hits, int *artillery_unlocked, int *torpedo_unlocked) {
    if (successful_hits >= 5 && *artillery_unlocked == 0) {
        *artillery_unlocked = 1;
        printf("Artillery unlocked!\n");
    }
    if (successful_hits >= 10 && *torpedo_unlocked == 0) {
        *torpedo_unlocked = 1;
        printf("Torpedo unlocked!\n");
    }
}
void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void place_ships_for_player(char board[GRID_SIZE][GRID_SIZE], Player *player)
{
    Ship ships[] = {
        {"Carrier", CARRIER_SIZE, {{0}}},
        {"Battleship", BATTLESHIP_SIZE, {{0}}},
        {"Destroyer", DESTROYER_SIZE, {{0}}},
        {"Submarine", SUBMARINE_SIZE, {{0}}}};

    printf("%s, place your ships on the grid!\n", player->name);
    for (int i = 0; i < TOTALNUMBEROFSHIPS; i++)
    {
        int valid = 0;
        while (!valid)
        {
            char column;
            int row;
            char orientation[10];

            printf("Enter the starting coordinates and orientation (horizontal or vertical) for your %s (size %d):\n", ships[i].name, ships[i].size);
            printf("Example: B3 horizontal\n");
            scanf(" %c%d %s", &column, &row, orientation);

            int col_index = column_to_index(column);
            int row_index = row - 1;
            char orient = (toupper(orientation[0]) == 'H') ? 'H' : 'V';

            if (row_index >= 0 && row_index < GRID_SIZE && col_index >= 0 && col_index < GRID_SIZE &&
                can_place_ship(board, row_index, col_index, ships[i].size, orient))
            {
                place_ship(board, row_index, col_index, ships[i].size, orient, ships[i].name[0]);
                valid = 1;
            }
            else
            {
                printf("Invalid ship placement. Try again.\n");
            }
        }
    }

    printf("%s has finished placing their ships.\n", player->name);
    clear_screen();
}

void player_turn(char opponent_board[GRID_SIZE][GRID_SIZE], Player *player)
{
    int valid = 0;
    while (!valid)
    {
        char column;
        int row;

        printf("%s, enter coordinates to strike (e.g., B3): ", player->name);
        scanf(" %c%d", &column, &row);

        int col_index = column_to_index(column);
        int row_index = row - 1;

        if (row_index >= 0 && row_index < GRID_SIZE && col_index >= 0 && col_index < GRID_SIZE)
        {
            if (opponent_board[row_index][col_index] == '~')
            {
                opponent_board[row_index][col_index] = 'O';
                printf("Miss\n");
                valid = 1;
            }
            else if (opponent_board[row_index][col_index] != 'X' && opponent_board[row_index][col_index] != 'O')
            {
                opponent_board[row_index][col_index] = 'X';
                printf("Hit\n");
                valid = 1;
            }
            else
            {
                printf("You already targeted this location. Try again.\n");
            }
        }
        else
        {
            printf("Invalid coordinates. Try again.\n");
        }
    }
}

int check_win(char board[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (board[i][j] != '~' && board[i][j] != '*' && board[i][j] != 'O')
                return 0;
        }
    }
    return 1;
}

int isGameOver(Player *player1, Player *player2)
{
    if (player1->ships_remaining == 0)
    {
        printf("Player 2 wins!\n");
        return 1;
    }
    else if (player2->ships_remaining == 0)
    {
        printf("Player 1 wins!\n");
        return 1;
    }
    return 0;
}

void endGame(Player *winner)
{
    if (winner == NULL)
    {
        printf("It's a draw!\n");
    }
    else
    {
        printf("Congratulations! %s has won the game!\n", winner->name);
    }
    printf("Thank you for playing!\n");
}

void gameLoop(Player *player1, Player *player2)
{
    int turn = 0;
    while (1)
    {
        if (turn % 2 == 0)
        {
            printf("\n%s's turn. Opponent's grid:\n", player1->name);
            display_opponent_grid(player2->hits, 'E');
            displayAvailableMoves(player1);
            player_turn(player2->hits, player1);
            if (check_win(player2->hits))
            {
                endGame(player1);
                break;
            }
        }
        else
        {
            printf("\n%s's turn. Opponent's grid:\n", player2->name);
            display_opponent_grid(player1->hits, 'E');
            displayAvailableMoves(player2);
            player_turn(player1->hits, player2);
            if (check_win(player1->hits))
            {
                endGame(player2);
                break;
            }
        }
        if (isGameOver(player1, player2))
        {
            if (player1->ships_remaining == 0 && player2->ships_remaining == 0)
            {
                endGame(NULL); // It's a draw
            }
            else
            {
                endGame(player1->ships_remaining > 0 ? player1 : player2);
            }
            break;
        }
        turn++;
    }
}
