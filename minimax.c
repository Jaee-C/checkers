#include "board.h"

node_t *make_root_node(board_t);
void insert_new_node(node_t *, data_t *);
void fill_tree(char, node_t *, int);
void leaf_cost(node_t *, int);
int minimax(node_t *, char, int);
data_t *choose_move(node_t *, int);
void free_tree(node_t *);

void perform_next_action(board_t board, char player, int count, int level) {
    node_t *root = NULL;
    int best_cost, tree_depth = level * LEVEL_MULTIPLIER;
    data_t *best_move;
    char action[MOVELEN+1];

    // Create and fill tree
    root = make_root_node(board);
    fill_tree(player, root, tree_depth);
    leaf_cost(root, tree_depth);

    best_cost = minimax(root, player, tree_depth);

    // Program terminates when there's a winner
    if (best_cost == INT_MAX) {
        printf("BLACK WIN!\n");
        exit(EXIT_SUCCESS);
    } else if (best_cost == INT_MIN) {
        printf("WHITE WIN!\n");
        exit(EXIT_SUCCESS);
    }

    best_move = choose_move(root, best_cost);

    // Remove captured piece, then update piece move
    check_error(board, best_move->src, best_move->dest, player);
    update_board(board, best_move->src, best_move->dest);

    // Print action
    get_action_name(best_move->src, best_move->dest, action);
    print_move(board, count, action, player, COMPUTE);

    // Tree is not used any more, can free
    free_tree(root);
    root = NULL;
}

/* Creates the root node for a tree */
node_t *make_root_node(board_t b) {
	node_t *root = NULL;
	root = malloc(sizeof(node_t));
    assert(root);
    root->child = NULL;    // Initiate child nodes to NULL
    root->child_num = 0;   // Root node initially has 0 child nodes
    root->curr_size = 0;   // Initially has size for 0 child nodes
    root->data = malloc(sizeof(data_t));
    assert(root->data);
    // Copy current board into root of tree
    duplicate_board(b, root->data->board);
    return root;
}


/* Create a new node for `option`, then inserts the new node into the tree */
void insert_new_node(node_t *root, data_t *option) {
    node_t *new;
    new = malloc(sizeof(node_t));
    assert(new);
    new->data = option;    // new node now points to `option`
    // New node does not have any child nodes, nor can it store any child nodes
    new->child = NULL;
    new->child_num = 0;
    new->curr_size = 0;

    if (!root->child) {
        // Root node does not have any child nodes
        root->curr_size = INITIAL;
        root->child = (node_t **) malloc(root->curr_size * sizeof(node_t *));
        assert(root->child);
        for (int i = 0; i < root->curr_size; i++) {
            root->child[i] = NULL;
        }
    } else if (root->child_num == root->curr_size) {
        // Root node has child nodes but cannot add any more child nodes
        root->curr_size *= MULTIPLIER;
        root->child = (node_t **)realloc(root->child,
                                         root->curr_size * sizeof(node_t *));
        assert(root->child);
    }

    // Adds new node to root node
    root->child[root->child_num] = new;
    root->child_num += 1;
}

/* Fills the tree of depth `curr_depth` starting from node `root` */
void fill_tree(char player, node_t *root, int curr_depth) {
    locn_t pos;
    data_t *option;
    int child_num=0;

    if (curr_depth == 0) {
        // Reached leaf node, no need to fill
        return;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (tolower(root->data->board[i][j]) == player) {
                pos.col = j;
                pos.row = i;

                // For each cell, check for valid moves in 4 directions
                for (int dir = 0; dir <= NORTHWEST; dir++) {
                    if ((option = is_valid_move(root->data->board, 
                                                pos, dir, player)) != 0) {
                        // Valid move is found, insert into tree
                        insert_new_node(root, option);
                        // Then, find all options from this move
                        if (curr_depth > 1) {
                            fill_tree(change_player(player),
                                      root->child[child_num], curr_depth - 1);
                            child_num++;
                        }
                    }
                }
            }
        }
    }
}

/* Cost all leaf nodes and store cost into node */
void leaf_cost(node_t *root, int depth) {
    if (depth == 0) {
        // Reached leaf node, add cost into node
        root->data->cost = calculate_cost(root->data->board);
    } else {
        for (int i = 0; i < root->child_num; i++) {
            leaf_cost(root->child[i], depth-1);
        }
    }
}

/* Implements the minimax algorithm, returns the optimal cost */
int minimax(node_t *root, char player, int depth) {
    int min_cost, max_cost, curr_cost;
    if (depth == 0) {
        // Reach leaf node, return cost
        return root->data->cost;
    }

    if (player == CELL_BPIECE) {    // Black player wants to maximise cost
        max_cost = INT_MIN;
        for (int i = 0; i < root->child_num; i++) {
            curr_cost = minimax(root->child[i], CELL_WPIECE, depth - 1);
            if (curr_cost > max_cost) {  // Get the highest cost of child nodes
                max_cost = curr_cost;
            }
        }
        // Propagate highest cost of child nodes to current node
        root->data->cost = max_cost;
        return root->data->cost;
    } else {                        // White player wants to minimise cost
        min_cost = INT_MAX;
        for (int i = 0; i < root->child_num; i++) {
            curr_cost = minimax(root->child[i], CELL_BPIECE, depth - 1);
            if (curr_cost < min_cost) {   // Get the lowest cost of child nodes
                min_cost = curr_cost;
            }
        }
        // Propagate lowest cost of child nodes to current node
        root->data->cost = min_cost;
        return root->data->cost;
    }
}

/* Find the first child node from all child nodes of `root` where cost=`cost` */
data_t *choose_move(node_t *root, int cost) {
    for (int i = 0; i < root->child_num; i++) {
        if (root->child[i]->data->cost == cost) {
            // Program returns the first instance where cost=`cost`
            return root->child[i]->data;
        }
    }
    return 0;
}


/* Free all nodes in the tree, including root node */
void free_tree(node_t *root) {
    if (root) {
        // Free all child nodes first
        for (int i = 0; i < root->child_num; i++) {
            free_tree(root->child[i]);
        }
        // Then free root node
        free(root->data);
        free(root);
        root = NULL;
    } else {
        return;
    }
}
