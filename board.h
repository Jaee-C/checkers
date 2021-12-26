#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

/* #define's -----------------------------------------------------------------*/
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
#define CAPITAL_A           'A'     // upper-case 'A' character
#define CAPITAL_P           'P'     // upper-case 'P' character
#define NORTH               -1      // north direction of action
#define SOUTH               1       // south direction of action
#define EAST                1       // east direction of action
#define WEST                -1      // west direction of action
#define RETURN_SUCCESS      1       // function call executed successfully
#define RETURN_FAILURE      0       // function call failed
#define NUM_OF_DIR          4       // number of directions

#define ACTION_INPUT                        0   // input action
#define ACTION_COMPUTED                     1   // computed action
#define ACTION_LEGAL                        0   // legal action
#define ACTION_ILLEGAL_SRC_OUTSIDE          1   // source cell outside board
#define ACTION_ILLEGAL_TGT_OUTSIDE          2   // target cell outside board
#define ACTION_ILLEGAL_SRC_EMPTY            3   // source cell is empty
#define ACTION_ILLEGAL_TGT_NOT_EMPTY        4   // target cell is not empty
#define ACTION_ILLEGAL_SRC_HOLDS_OPPONENT   5   // source cell holds opponent
#define ACTION_ILLEGAL_OTHER                6   // other illegal action
#define INPUT_UNKNOWN                       0   // unknown input command
#define INPUT_ACTION                        1   // input action command
#define INPUT_COMMAND_COMPUTE_ACTION        2   // input compute action command
#define INPUT_COMMAND_PLAY                  3   // input play game command

#define STR_BOARD_SIZE              "BOARD SIZE: %dx%d\n"
#define STR_BLACK_PIECES_STATS      "#BLACK PIECES: %d\n"
#define STR_WHITE_PIECES_STATS      "#WHITE PIECES: %d\n"
#define STR_DELIMITER               "=====================================\n"
#define STR_COMPUTED_ACTION_MARKER  "*** "
#define STR_WHITE_ACTION            "WHITE ACTION #%d: "
#define STR_BLACK_ACTION            "BLACK ACTION #%d: "
#define STR_ACTION_INFO             "%c%d-%c%d\n"
#define STR_ACTION_INFO_SFX         "%d-%c%d\n"
#define STR_BOARD_COST              "BOARD COST: %d\n"
#define STR_BLACK_WIN               "BLACK WIN!\n"
#define STR_WHITE_WIN               "WHITE WIN!\n"
#define STR_DOUBLE_SPACE            "  "
#define STR_NEW_LINE                "\n"
#define STR_BOARD_COL_TITLE         "   %c"
#define STR_BOARD_ROW_TITLE         "%2d |"
#define STR_BOARD_ROW_INDENT        "\n   +"
#define STR_BOARD_ROW_DELIM         "---+"
#define STR_BOARD_CELL_CONTENT      " %c |"

#define STR_ERROR_1         "ERROR: Source cell is outside of the board.\n"
#define STR_ERROR_2         "ERROR: Target cell is outside of the board.\n"
#define STR_ERROR_3         "ERROR: Source cell is empty.\n"
#define STR_ERROR_4         "ERROR: Target cell is not empty.\n"
#define STR_ERROR_5         "ERROR: Source cell holds opponent's piece/tower.\n"
#define STR_ERROR_6         "ERROR: Illegal action.\n"

// two useful global arrays
int LAT[NUM_OF_DIR] = {NORTH,SOUTH,SOUTH,NORTH};
int LON[NUM_OF_DIR] = {EAST,EAST,WEST,WEST};

/* type definitions ----------------------------------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type
typedef struct action action_t;                         // action type
struct action { // an action is determined by source and target rows and columns
    int srow, scol;             // source row 'srow' and column 'scol'
    int trow, tcol;             // target row 'trow' and column 'tcol'
};
typedef struct node node_t;   // board node (in a minimax tree) type
struct node {                  // board node
    action_t    act;            // action that results in this board node
    board_t     brd;            // board that results from taking 'act'
    int         cst;            // cost of this node in the minimax tree
    int         num;            // upper bound on the number of reachable boards
    node_t     **chrn;         // childrens of this node in the minimax tree
};

/* function prototypes -------------------------------------------------------*/
void initialize_board(board_t);               // initialize input board
int  count_cells(board_t,char);               // count input board cells
void print_board(board_t);                    // print input board to stdout
void print_row_delim(int);                    // print board deliminter
int  read_input_cmd(action_t*);               // read command from stdin
void print_action(board_t,action_t*,int,int); // print action info to stdout
int  check_action(board_t,action_t*,int);     // check action
void take_action(board_t,action_t*);          // take action in the board
int  get_board_cost(board_t);                 // compute cost of the board
void print_error(int);                        // print action error status
int  is_on_board(int,int);                    // check if cell is on the board
int  compute_action(board_t,action_t*,int);   // compute next action
void construct_tree(node_t*,int,int,int);    // construct minimax tree
void construct_child_node(node_t*,int,       // construct child tree node
        action_t*,int,int);
void free_tree(node_t*);                     // free minimax tree
void set_tree_costs(node_t*,int,int);    // set minimax tree node costs
void clone_board(board_t,board_t);            // clone board
void print_winner(int);                       // print winner info
