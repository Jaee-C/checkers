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

int main(int argc, char *argv[]) {
    // YOUR IMPLEMENTATION OF STAGES 0-2
    board_t board;
    locn_t source, target;
    char action[MOVELEN+1], player = CELL_BPIECE;
    int input_len, count = 0, error = 0;

    board_init(board);

    while ((input_len = get_input(action)) != EOF && input_len == MOVELEN) {
        // loop stops when non-move input ('A' or 'P') is read
        // Player's move
        count++;

        process_input(action, &source, &target);
        error = check_error(board, source, target, player);
        print_error(error);

        update_board(board, source, target);

        print_move(board, count, action, player, INPUT);
        player = change_player(player);   // After every move, change player

        count++;
        bot_move(board, &count, &player);
    }

    if (input_len == 1 && action[0] == 'A') {
        // Get one action
        count++;
        perform_next_action(board, player, count);
    } else if (input_len == 1 && action[0] == 'P') {
        // Get next COMP_ACTIONS actions
        for (int i = 0; i < COMP_ACTIONS; i++) {
            count++;
            perform_next_action(board, player, count);
            player = change_player(player);
        }
    }

    return EXIT_SUCCESS;  // exit program with the success code
}

/* THE END -------------------------------------------------------------------*/
// algorithms are fun
