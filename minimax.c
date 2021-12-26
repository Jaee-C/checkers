#include "board.h"

// compute the next action in the game defined by the board and the next turn
int
compute_action(board_t B, action_t* A, int turn) {
    assert(A!=NULL && B!=NULL);
    node_t* root=(node_t*)malloc(sizeof(*root));  // root of the minimax tree
    assert(root!=NULL);
    clone_board(B,root->brd);                       // input board becomes root
    construct_tree(root,TREE_DEPTH,turn,turn);      // construct minimax tree
    set_tree_costs(root,turn,turn);                 // set minimax node costs
    int i;
    for (i=0; i<root->num; i++) {
        if (root->chrn[i]!=NULL && root->cst==root->chrn[i]->cst) {
            *A=root->chrn[i]->act;      // set computed action
            free_tree(root);            // free allocated memory
            return RETURN_SUCCESS;      // next action successfully computed
        }
    }
    free_tree(root);                    // free allocated memory
    return RETURN_FAILURE;              // next action does not exist
}

// set minimax tree node costs
void
set_tree_costs(node_t* node, int turn, int iturn) {
    assert(node!=NULL);
    if (turn-iturn==TREE_DEPTH) {
        node->cst = get_board_cost(node->brd);  // set leaf node cost
        return;                                 // prune recursion
    }
    if (turn%2) {           // white turn
        node->cst=INT_MAX;  // initialize node cost
    } else {                // black turn
        node->cst=INT_MIN;  // initialize node cost
    }

    int i;
    for (i=0; i<node->num; i++) { // iterate over children and update cost
        if (node->chrn[i]!=NULL) {
            set_tree_costs(node->chrn[i],turn+1,iturn);   // recurse
            // the following two if statements can be rewritten into one
            // compact, but less readable, if statement (not done here)
            if (turn%2 && node->chrn[i]->cst<node->cst) {
                node->cst=node->chrn[i]->cst;   // white aim to minimize cost
            }
            if (!(turn%2) && node->chrn[i]->cst>node->cst) {
                node->cst=node->chrn[i]->cst;   // black aim to maximize cost
            }
        }
    }
}

// setup board 'to' in the same way as board 'from' is setup
void
clone_board(board_t from, board_t to) {
    int row, col;
    for (row=0; row<BOARD_SIZE; row++) {
        for (col=0; col<BOARD_SIZE; col++) {
            to[row][col] = from[row][col];  // copy cell from 'from' to 'to' :)
        }
    }
}

// print winner
void
print_winner(int turn) {
    if (turn%2) {
        printf(STR_BLACK_WIN);      // black win!
    } else {
        printf(STR_WHITE_WIN);      // white win!
    }
}

// construct minimax tree
void
construct_tree(node_t* node, int depth, int turn, int iturn) {
    assert(node!=NULL);
    int i, j, delta, status, k=0;
    // get the bound on possible action directions for the player with the turn
    if (turn%2) {
        node->num = NUM_OF_DIR*(count_cells(node->brd,CELL_WPIECE)+
                        count_cells(node->brd,CELL_WTOWER));
    } else {
        node->num = NUM_OF_DIR*(count_cells(node->brd,CELL_BPIECE)+
                        count_cells(node->brd,CELL_BTOWER));
    }

    // allocate and initialize array of children
    node->chrn = (node_t**) malloc((node->num)*sizeof(node_t*));
    assert(node->chrn!=NULL);
    for (i=0; i<node->num; i++) { 
        node->chrn[i]=NULL; // initialize to NULLs, or use calloc() instead :)
    }

    if (depth<=0) { // if the requested depth of recursive calls is reached ...
        return;     // ... prune recursion
    }

    // attempt all posisble actions and construct child sub-trees for legal ones
    action_t A;
    for (i=0; i<BOARD_SIZE; i++) {            // traverse cells of the board ...
        for (j=(i+1)%2; j<BOARD_SIZE; j+=2) {   // ... in row-major order
            // check if the cell can be skipped
            if ((node->brd[i][j]==CELL_EMPTY) || // cell is empty
                ((node->brd[i][j]==CELL_BPIECE || node->brd[i][j]==CELL_BTOWER)
                  && turn%2) ||                  // black element for white turn
                ((node->brd[i][j]==CELL_WPIECE || node->brd[i][j]==CELL_WTOWER)
                && !(turn%2))) {                 // white element for black turn
                continue;                        // skip to the next cell
            }

            // construct children by attempting actions starting from the 
            // north-east direction and proceeding clockwise
            for (delta=0; delta<NUM_OF_DIR; delta++) {
                A.srow=i; A.scol=j; A.trow=i+LAT[delta]; A.tcol=j+LON[delta];
                if ((status=check_action(node->brd,&A,turn))==ACTION_LEGAL) {
                    // move is legal -> construct corresponding child node
                    construct_child_node(node,NUM_OF_DIR*k+delta,&A,depth,turn);
                } else if (status==ACTION_ILLEGAL_TGT_NOT_EMPTY) {
                    // a capture may still be possible in this direction
                    A.trow+=LAT[delta]; A.tcol+=LON[delta];
                    if (check_action(node->brd,&A,turn)==ACTION_LEGAL) {
                        // yes, capture is indeed possible -> construct child
                        construct_child_node(node,NUM_OF_DIR*k+delta,
                                                &A,depth,turn);
                    }
                }
            }

            k++;
        }
    }
}

// construct child node in a minimax tree
void 
construct_child_node(node_t* node, int cid, action_t* A, int depth, int turn) {
    assert(node!=NULL && A!=NULL);
    node->chrn[cid]=(node_t*)malloc(sizeof(node_t));
    assert(node->chrn[cid]!=NULL);
    clone_board(node->brd,node->chrn[cid]->brd);                // clone board
    node->chrn[cid]->act=*A;                                    // set action
    take_action(node->chrn[cid]->brd,&node->chrn[cid]->act);    // take action
    // make a recursive call to construct the child subtree
    construct_tree(node->chrn[cid],depth-1,turn+1,turn);
}

// free memeory allocated for the minimax tree
void
free_tree(node_t* node) {
    assert(node!=NULL);
    int i;
    for (i=0; i<node->num; i++) {
        if (node->chrn[i]!=NULL) {
            free_tree(node->chrn[i]);   // free child subtree
        }
    }
    free(node->chrn);                   // free children
    free(node);                         // free input node
}
