/*
 * NAME: Lucas Chagas
 * COURSE: CS230 / Spring, 2019
 *
 * DESCRIPTION:
 *
 * This implementation of 'Connect-4' uses a multi-dim array to represent
 * the game board. I use several functions, some of which take in an array
 * as a parameter. To print out the board, I traverse the array and map the
 * numbers (0, 1, 2) to (".", "X","O"). My implementation also includes three
 * different end-game states (win, lose, or draw). An AI (that plays at a
 * 5th grade level) is included to be the player's opponent. No global
 * variables were used for my implementation. The player can choose to go
 * first or second. There is also a 'move list' displayed at the end of each
 * game.
 *
 * NOTE: Video demo is sped up to meet 1 minute requirement.
 */

#include <stdbool.h>
#include <stdio.h>

// Function prototypes
int drop_piece(int board[][7], int row, int col, int piece);
void print_board(int board[][7]);
int char_to_int(char c);
char int_to_char(int n);
bool is_valid_location(int board[][7], int col);
int get_next_open_row(int board[][7], int col);
int check_possible_defence(int board[][7], int piece);
int check_possible_win(int board[][7], int piece);
int get_attack_move(int board[][7], int piece);
int check_possible_win(int board[][7], int piece);
int min(int a[], int size);


int main(void) {
  // Create Board
  int board[6][7] = {{0, 0, 0, 0, 0, 0, 0},
		     {0, 0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0, 0},
		     {0, 0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0, 0},
		     {0, 0, 0, 0, 0, 0, 0}};

  bool game_over = false; // Game state
  int turn = 0;         // Variable to help alternate between the AI and player
  int moves = 0;        // Total moves made
  char move_list[42];   // List of recorded moves
  int player_piece = 1; // Assume player wants to go first
  int ai_piece = 2;     // Assume that AI will go second

  printf("Would you like to go first or second? Type 1 or 2: ");
  scanf("%i", &turn);

  // Assigns who moves first
  if (turn == 2) {
    player_piece = 2;
    ai_piece = 1;
  }
  turn--;

  // Games continues to loop until there is a winner or a draw
  while (!game_over) {
    int winner = -1;

    if (moves == 42) {
      print_board(board);
      printf("The game is a draw!");
      break;
    }

    if (turn == 0) {
      // Player Moves Here
      print_board(board);
      int col;
      char char_col;
      printf("Please choose a column A-G: ");
      scanf(" %c", &char_col);
      col = char_to_int(char_col);
      if (is_valid_location(board, col)) {
        int row = get_next_open_row(board, col);
        winner = drop_piece(board, row, col, player_piece);
        move_list[moves] = int_to_char(col);
      }
    } else {
      // Drops in the middle if AI plays first
      // Ignores other strategies
      if (moves == 0) {
        drop_piece(board, 0, 3, ai_piece);
      } else {
        // AI Moves Here
        // Gets index for defensive strategy if there is one.
        int defence = check_possible_defence(board, ai_piece);
        // Gets index for winning strategy to win the game in a single move
        int win = check_possible_win(board, ai_piece);
        // Gets index of attacking strategy in an attempt to hurt the chances of
        // the human player winning.
        int attack = get_attack_move(board, ai_piece);
        int row; // Row index
        int col; // Column index

        // First check if you can win
        // If you can't, check if you have to defend
        // Else, look for a move that decreases the human player's possible
        // 4-connections
        if (win != -1) {
          row = get_next_open_row(board, win);
          col = win;
        } else if (defence != -1) {
          row = get_next_open_row(board, defence);
          col = defence;
        } else {
          row = get_next_open_row(board, attack);
          col = attack;
        }
        winner = drop_piece(board, row, col, ai_piece);
        move_list[moves] = int_to_char(col);
      }
    }

    // The next two lines alternate the game between the player and the AI
    turn++;
    turn %= 2;
    moves++;

    // When a winner is found
    if (winner != -1) {
      game_over = true;
      print_board(board);
      if (winner == ai_piece) {
        printf("YOU LOST! AI WINS!\n");
      } else {
        printf("YOU WON!\n");
      }
      printf("Moves: ");
      for (int x = 0; x < moves; x++) {
        printf("%c ", move_list[x]);
      }
    }
  }

  return 0;
}

// Prints the board
void print_board(int board[][7]) {

  printf("\e[2J\e[0;0H");
  printf("\n");
  printf("\tA\tB\tC\tD\tE\tF\tG\n");

  for (int r = 5; r >= 0; r--) {
    printf("%i\t", r + 1);

    for (int c = 0; c <= 6; c++) {
      if (board[r][c] == 1) {
        printf("X\t");
      } else if (board[r][c] == 2) {
        printf("O\t");
      } else {
        printf(".\t");
      }
    }
    printf("\n");
  }

  printf("-----------------------------\n");
}

// Maps the letters (A-G) to (0-6)
int char_to_int(char c) {
  int col;

  switch (c) {
  case 'A':
    col = 0;
    break;
  case 'B':
    col = 1;
    break;
  case 'C':
    col = 2;
    break;
  case 'D':
    col = 3;
    break;
  case 'E':
    col = 4;
    break;
  case 'F':
    col = 5;
    break;
  case 'G':
    col = 6;
    break;
  default:
    break;
  }

  return col;
}

// Maps the letters (0-6) to (A-G)
char int_to_char(int n) {
  char col;

  switch (n) {
  case 0:
    col = 'A';
    break;
  case 1:
    col = 'B';
    break;
  case 2:
    col = 'C';
    break;
  case 3:
    col = 'D';
    break;
  case 4:
    col = 'E';
    break;
  case 5:
    col = 'F';
    break;
  case 6:
    col = 'G';
    break;
  default:
    break;
  }

  return col;
}

// Inserts a piece into the board. Also checks to see if the game is over
int drop_piece(int board[][7], int row, int col, int piece) {
  bool winning_move(int board[][7], int piece);
  board[row][col] = piece;
  if (winning_move(board, piece)) {
    return piece;
  }

  return -1;
}

// Removes a piece from the board
void lift_piece(int board[][7], int row, int col) { board[row][col] = 0; }

// Checks to see if a column is not full
bool is_valid_location(int board[][7], int col) { return board[5][col] == 0; }

// Gets the row index of a given column. Given that the column is not full.
int get_next_open_row(int board[][7], int col) {
  int r;

  for (r = 0; r < 6; r++) {
    if (board[r][col] == 0) {
      break;
    }
  }

  return r;
}

// Checks to see if a given piece has one the game
// based on the current state of the game.
bool winning_move(int board[][7], int piece) {
  // Horizontal Win
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 6; r++) {
      if (board[r][c] == piece && board[r][c + 1] == piece &&
          board[r][c + 2] == piece && board[r][c + 3] == piece) {
        return true;
      }
    }
  }

  // Verticle Win
  for (int c = 0; c < 7; c++) {
    for (int r = 0; r < 3; r++) {
      if (board[r][c] == piece && board[r + 1][c] == piece &&
          board[r + 2][c] == piece && board[r + 3][c] == piece) {
        return true;
      }
    }
  }

  // Positive Slope Win
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 3; r++) {
      if (board[r][c] == piece && board[r + 1][c + 1] == piece &&
          board[r + 2][c + 2] == piece && board[r + 3][c + 3] == piece) {
        return true;
      }
    }
  }

  // Negative Slope Win
  for (int c = 0; c < 4; c++) {
    for (int r = 3; r < 6; r++) {
      if (board[r][c] == piece && board[r - 1][c + 1] == piece &&
          board[r - 2][c + 2] == piece && board[r - 3][c + 3] == piece) {
        return true;
      }
    }
  }

  return false;
}

/*
Given the state the board is current in, this function finds
the total number of ways the current player could achieve
a 4-connection win scenario.
*/
int count_possible_connections(int board[][7], int piece) {
  int counter = 0;

  // Counts Horizontal Wins
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 6; r++) {
      if ((board[r][c] == piece || board[r][c] == 0) &&
          (board[r][c + 1] == piece || board[r][c + 1] == 0) &&
          (board[r][c + 2] == piece || board[r][c + 2] == 0) &&
          (board[r][c + 3] == piece || board[r][c + 3] == 0)) {
        counter++;
      }
    }
  }

  // Counts Verticle Wins
  for (int c = 0; c < 7; c++) {
    for (int r = 0; r < 3; r++) {
      if ((board[r][c] == piece || board[r][c] == 0) &&
          (board[r + 1][c] == piece || board[r + 1][c] == 0) &&
          (board[r + 2][c] == piece || board[r + 2][c] == 0) &&
          (board[r + 3][c] == piece || board[r + 3][c] == 0)) {
        counter++;
      }
    }
  }

  // Counts Positive Slope Wins
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 3; r++) {
      if ((board[r][c] == piece || board[r][c] == 0) &&
          (board[r + 1][c + 1] == piece || board[r + 1][c + 1] == 0) &&
          (board[r + 2][c + 2] == piece || board[r + 2][c + 2] == 0) &&
          (board[r + 3][c + 3] == piece || board[r + 3][c + 3] == 0)) {
        counter++;
      }
    }
  }

  // Counts Negative Slope Wins
  for (int c = 0; c < 4; c++) {
    for (int r = 3; r < 6; r++) {
      if ((board[r][c] == piece || board[r][c] == 0) &&
          (board[r - 1][c + 1] == piece || board[r - 1][c + 1] == 0) &&
          (board[r - 2][c + 2] == piece || board[r - 2][c + 2] == 0) &&
          (board[r - 3][c + 3] == piece || board[r - 3][c + 3] == 0)) {
        counter++;
      }
    }
  }

  return counter;
}

// Simulate human player making each possible move and determine if they can
// win. If they can win, move to block them
int check_possible_defence(int board[][7], int piece) {
  // Determine if the human player is X or O
  int player_piece;

  if (piece == 1) {
    player_piece = 2;
  } else {
    player_piece = 1;
  }
  return check_possible_win(board, player_piece);
}

// Checks to see if a given piece can win in a single move.
// Returns the column of the winning move if there is one.
int check_possible_win(int board[][7], int piece) {
  for (int c = 0; c < 7; c++) {
    if (is_valid_location(board, c)) {
      int row = get_next_open_row(board, c);

      drop_piece(board, row, c, piece);

      // check to see if the human player has won
      if (winning_move(board, piece)) {
        lift_piece(board, row, c);
        return c;
      }
      lift_piece(board, row, c);
    }
  }

  return -1;
}

// Finds an aggresive strategy to combat the human player
int get_attack_move(int board[][7], int piece) {
  /*
  Keeps track of the human player's possible 4-connection
  win scenarios. Set to 100 be default to force the AI
  not to pick a row that's already full.
  */
  int human_ratings[7] = {100, 100, 100, 100, 100, 100, 100};

  // Determine if the human player is X or O
  int player_piece;
  if (piece == 1) {
    player_piece = 2;
  } else {
    player_piece = 1;
  }

  /*
  Goes through each possible option for the AI. For each option,
  the AI assumes the human player is rational. Through this
  assumption, the AI determines which column limits the number
  of possible 4-connection win scenarios for the human player.
  However, the AI ignores the possibility of further restricting
  its own 4-connection win scenarios which may lead to the human
  player gaining an advantage.
  */
  for (int c = 0; c < 7; c++) {
    if (is_valid_location(board, c)) {
      int row = get_next_open_row(board, c);
      drop_piece(board, row, c, piece);

      // Now check player responses AI simulated drop piece
      int max_count = 0;
      for (int c_p = 0; c_p < 7; c_p++) {
        if (is_valid_location(board, c_p)) {
          int row_p = get_next_open_row(board, c_p);
          drop_piece(board, row_p, c_p, player_piece);
          int temp_count = count_possible_connections(board, player_piece);
          if (temp_count > max_count) {
            max_count = temp_count;
          }
          lift_piece(board, row_p, c_p);
        }
      }
      human_ratings[c] = max_count;

      lift_piece(board, row, c);
    }
  }

  return min(human_ratings, 7);
}

// Finds the smallest element in an array and returns the index
int min(int a[], int size) {
  // Sets the minimum to the first element in the array
  int minimum = a[0];
  // Keeps track of the index for the current minimum
  int index = 0;

  for (int x = 1; x < size; x++) {
    if (a[x] < minimum) {
      minimum = a[x];
      index = x;
    }
  }

  return index;
}
