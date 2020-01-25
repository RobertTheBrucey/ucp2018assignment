/*****************************************************************************
* File:      FileInput.h                                                     *
* Author:    Sean Ashton * Student ID:                               *
* Unit:      COMP1000 Unix and C Programming                                 *
* Purpose:   Header for file input, includes structs for commands            *
* Reference: None.                                                           *
* Comments:  This comment block is the maximum width of 78 characters.       *
* Requires:  Nothing. List dependencies here                                 *
* Created:   04/10/2018 * Last Modified: 15/10/2018                          *
*****************************************************************************/
#ifndef FILEINPUT_H
#define FILEINPUT_H
#include "LinkedList.h"
/* Use a single struct type for all input, this is still more efficient than
    storing a string for each command, though I would prefer to have different
    structs and a robust method for checking what kind is present. */

typedef struct Command {
    char type;
    union {/*RAM usage is the same as when not using a union :(*/
        double real;
        int integer;
        char pattern;
    } data;
} Command;

/* read in lines from a file, error codes:
    0: Success, 1: NULL filename, 2: Failed to open file,
    3: Invalid format, 4: Error with inserting to list */
int readLines( char*, LinkedList* );
void readError( int error );
#endif
