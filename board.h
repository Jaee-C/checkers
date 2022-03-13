#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions

#define ROW_SEP             "+---+---+---+---+---+---+---+---+"
#define DELIMITER           "====================================="

// ASCII conversion of char to int
#define ALPHA_TO_INT        65      // conversion from alphabet to int
#define NUM_TO_INT          49      // conversion from numbers to int

// Error numbers
#define ERROR1              1
#define ERROR2              2
#define ERROR3              3
#define ERROR4              4
#define ERROR5              5
#define ERROR6              6

// Indicates the formatting of an action input
#define MOVELEN             5       // length of input for a move
#define SOURCE_COL          0       // column number for source cell
#define SOURCE_ROW          1       // row number for source cell
#define TARGET_COL          3       // column number for target cell
#define TARGET_ROW          4       // row number for target cell

// Directions for finding valid moves - ascending in clockwise direction
#define NORTHEAST           0
#define SOUTHEAST           1
#define SOUTHWEST           2
#define NORTHWEST           3

#define INITIAL             2       // initial size of child nodes array
#define MULTIPLIER          2
// The factor in which the size of child nodes array increases

#define INPUT               0       // move is inputted
#define COMPUTE             1       // move is computed

typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

typedef struct {
    int row, col;
} locn_t;

typedef struct {
    board_t board;
    locn_t src, dest;   // Action
    int cost;
} data_t;

typedef struct node node_t;

struct node {
    data_t *data;
    node_t **child;  // Child nodes
    int child_num;   // Current number of child nodes
    int curr_size;   // The maximum number of child nodes the node can point to
};

void board_init(board_t);
int cell_has_piece(int, int);
void print_board(board_t);
void update_board(board_t, locn_t, locn_t);
void print_move(board_t, int, char *, char, int);
char change_player(char);
void process_input(char *, locn_t *, locn_t *);
int calculate_cost(board_t);
int get_input(char action[], char);
int check_error(board_t, locn_t, locn_t, char);
int piece_capture(board_t, locn_t, locn_t, char);
int tower_capture(board_t, locn_t, locn_t, char);
void print_error(int);
node_t *make_root_node(board_t);
void duplicate_board(board_t, board_t);
void insert_new_node(node_t *, data_t *);
void fill_tree(char, node_t *, int);
data_t *is_valid_move(board_t, locn_t, int, char);
locn_t diagonal_move(locn_t, int);
void get_action_name(locn_t, locn_t, char *);
void leaf_cost(node_t *, int);
int minimax(node_t *, char, int);
void perform_next_action(board_t board, char color, int count);
data_t *choose_move(node_t *, int);
void free_tree(node_t *);
void bot_move(board_t, int*, char*);
