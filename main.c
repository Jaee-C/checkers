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

#include "board.h"

int main(int argc, char *argv[]) {
    if (ROWS_WITH_PIECES*2>BOARD_SIZE || ROWS_WITH_PIECES<1 || BOARD_SIZE<1) {
        return EXIT_FAILURE;    // bad initial setup (exit with failure)
    }

    board_t B;                  // chess board
    initialize_board(B);        // initialize board
    // print board info to stdout (size and numbers of black and white pieces)
    printf(STR_BOARD_SIZE,BOARD_SIZE,BOARD_SIZE);
    printf(STR_BLACK_PIECES_STATS,count_cells(B,CELL_BPIECE));
    printf(STR_WHITE_PIECES_STATS,count_cells(B,CELL_WPIECE));
    print_board(B);             // print board setup to stdout

    action_t A;                 // an action in the game
    int cmd, status, turn=0;
    while ((cmd=read_input_cmd(&A))==INPUT_ACTION) { // if action A was read
        status=check_action(B,&A,turn);              // check the status of A
        if (status==ACTION_LEGAL) { // if action A is legal ...
            take_action(B,&A);      // ... take action A in board B
            print_action(B,&A,turn,ACTION_INPUT);    // print action A info
        } else {                    // ... otherwise (A is illegal) ...
            print_error(status);    // print error message
            return EXIT_FAILURE;    // exit program with failure
        }
        turn++;                     // proceed to the next turn
    }

    if (cmd==INPUT_COMMAND_COMPUTE_ACTION) {         // if 'A' command was read
        if (compute_action(B,&A,turn)) {             // next action computed
            take_action(B,&A);                       // take action A in board B
            print_action(B,&A,turn,ACTION_COMPUTED); // print action A info
        } else {                                     // no next action available
            print_winner(turn);                      // print winner to stdout
        }
    }

    if (cmd==INPUT_COMMAND_PLAY) {                   // if 'P' command was read
        int count=0;                                 // # of computed actions
        while ((status=compute_action(B,&A,turn)) && ++count<=COMP_ACTIONS) {
            take_action(B,&A);                       // take action A in board B
            print_action(B,&A,turn,ACTION_COMPUTED); // print action A info
            turn++;                                  // proceed to the next turn
        }
        if (status==RETURN_FAILURE && count<COMP_ACTIONS) {
            print_winner(turn);                      // print winner to stdout
        }
    }
    return EXIT_SUCCESS;            // exit program with the success code
}

/* THE END -------------------------------------------------------------------*/
// algorithms are fun