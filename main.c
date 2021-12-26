/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Weng Jae Chin
  Dated:     1/10/2021

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "board.h"

// /* some #define's from my sample solution ------------------------------------*/
// #define BOARD_SIZE          8       // board size
// #define ROWS_WITH_PIECES    3       // number of initial rows with pieces
// #define CELL_EMPTY          '.'     // empty cell character
// #define CELL_BPIECE         'b'     // black piece character
// #define CELL_WPIECE         'w'     // white piece character
// #define CELL_BTOWER         'B'     // black tower character
// #define CELL_WTOWER         'W'     // white tower character
// #define COST_PIECE          1       // one piece cost
// #define COST_TOWER          3       // one tower cost
// #define TREE_DEPTH          3       // minimax tree depth
// #define COMP_ACTIONS        10      // number of computed actions

// /* one type definition from my sample solution -------------------------------*/
// typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

int main(int argc, char *argv[]) {
    // YOUR IMPLEMENTATION OF STAGES 0-2
    board_t board;
    locn_t source, target;
    char action[MOVELEN+1], color = CELL_BPIECE;
    int input_len, count = 0;
 
    board_init(board);

    while ((input_len = get_input(action)) != EOF && input_len == MOVELEN) {
        // loop stops when non-move input ('A' or 'P') is read

        process_input(action, &source, &target);
        check_input_error(board, source, target, color);

        // is there any capture or illegal actions?
        capture_check(board, source, target);

        update_board(board, source, target);

        count++;

        // Output updated board after every input
        print_move(board, count, action, &color);
    }
    return EXIT_SUCCESS;            // exit program with the success code
}

/* THE END -------------------------------------------------------------------*/
// algorithms are fun