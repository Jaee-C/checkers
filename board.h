/* some #define's from my sample solution ------------------------------------*/
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
#define DELIMITER_LEN       37
#define ALPHA_TO_INT        65
#define NUM_TO_INT          49
#define ERROR1              "Source cell is outside of the board."
#define ERROR2              "Target cell is outside of the board."
#define ERROR3              "Source cell is empty."
#define ERROR4              "Target cell is not empty."
#define ERROR5              "Source cell holds opponent’s piece/tower."
#define ERROR6              "Illegal action."

/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

typedef struct {
    int row, col;
} locn_t;

void print_head(void);
void board_init(board_t);
int cell_has_piece(int row, int col);
void print_board(board_t);
void print_delimiter(void);
void player_move(board_t board, char color);
int get_input(char action[]);
void check_input_error(board_t board, locn_t source, locn_t target, char c);