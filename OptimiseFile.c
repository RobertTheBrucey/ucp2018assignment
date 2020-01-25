/*****************************************************************************
* File:      OptimiseFile.c                                                  *
* Author:    Sean Ashton * Student ID: 14866636                              *
* Unit:      COMP1000 Unix and C Programming                                 *
* Purpose:   Optimise script made input file.                                *
* Reference: None.                                                           *
* Comments:  Continue on a many errors as possible.                          *
* Requires:  LinkedList.h, FileInput.h, stdio.h, stdlib.h                    *
* Created:   16/10/2018 * Last Modified: 16/10/2018                          *
*****************************************************************************/
#include "LinkedList.h"
#include "FileInput.h"
#include <stdio.h>
#include <stdlib.h>

/* Constants */
#define REAL_DEFAULT 0.0
#define FG_DEFAULT 7 /*Per the spec "white"*/
#define BG_DEFAULT 0 /*Per the spec, black*/
#define FG_MAX 15
#define BG_MAX 7
#define PATT_DEFAULT '+'
#define LOGSTR_LEN 101

/* Forward declarations */
int outputCommand( LinkedList *list, FILE* out );
int plot( LinkedList*, FILE* );

/*****************************************************************************
* Name: main
* Purpose: Do all the things. It's main.
* Imports: argc, argv, the number and contents of commandline arguments
* Exports: error code, 0 for success.
*****************************************************************************/
int main( int argc, char **argv )
{
    /* If nothing happens than there is still error output via main return */
    int readResult = -1;
    FILE* out;
    char* fileOut = (char*)malloc( LOGSTR_LEN * sizeof( char ) );
    LinkedList *list;
    /* I like heap based storage for passed things around */
    list = (LinkedList*)malloc( sizeof( LinkedList ) );
    list->head = NULL;
    list->tail = NULL;
    if ( argc != 2 )
    {
        fprintf( stderr, "Incorrect number of arguments. Usage:\n" );
        fprintf( stderr, "TurtleGraphics filename\n" );
    }
    else
    {
        readResult = readLines( argv[1], list );
        /* If there is a problem, output an appropriate error and exit */
        if ( readResult != 0 )
        {
            readError( readResult );
        }
        else
        {
            /* Set up for plotting image */
            sprintf(fileOut, "%s.P", argv[1]);
            printf( "Opening %s for output\n", fileOut );
            out = fopen(fileOut, "w");
            if ( out != NULL )
            {
                plot( list, out );
                fclose(out);
            }
            else
            {
                fprintf( stderr, "Problem opening output file\n" );
            }
        }
    }
    while ( list->head != NULL )/* If something bad happens, free everything*/
    {
        removeFirst( list );
        readResult = -2;/* Express an error had occured with return code */
    }
    free( list );
    return readResult;
}

/*****************************************************************************
* Name: plot
* Purpose: Call functions in effects.c to plot commands in a linked list
*           populated by FileInput.c
* Comments: Slightly longer than recommended due largely to conditional
            compilation options.
* Imports: LinkedList*, Pointer to the list that contains the commands to plot
* Export: error, error code in the event of a problem
*****************************************************************************/

int plot( LinkedList *list, FILE* out )
{
    /* Variable declarations and initialisation */
    double move, draw, ang;
    char patt;
    int fg, bg;
    Command *command;
    move = draw = ang = REAL_DEFAULT;
    patt = PATT_DEFAULT;
    fg = FG_DEFAULT;
    bg = BG_DEFAULT;

    while ( list->head != NULL )
    {
        command = (Command*)list->head->data;
        switch ( command->type )
        {
            case 'r':/*Rotate*/
            if (list->head->next != NULL)
            {
                ang += command->data.real;
                if (((Command*)list->head->next->data)->type != 'r')
                {
                    fprintf( out, "ROTATE %f\n", ang );
                    ang = 0.0;
                }
            }
            else
            {
                ang += command->data.real;
                fprintf( out, "ROTATE %f\n", ang );
            }

            break;

            case 'm':/*Move*/
            if (list->head->next != NULL)
            {
                move += command->data.real;
                if (((Command*)list->head->next->data)->type != 'm')
                {
                    fprintf( out, "MOVE %f\n", move );
                    move = 0.0;
                }
            }
            else
            {
                move += command->data.real;
                fprintf( out, "MOVE %f\n", move );
            }
            break;

            case 'p':/*Pattern*/
            if (command->data.pattern != patt)
            {
                patt = command->data.pattern;
                fprintf( out, "PATTERN %c\n", patt );
            }
            break;

            #ifndef SIMPLE
            case 'f':/*Foreground*/
            if (command->data.integer != fg)
            {
                fg = command->data.integer;
                fprintf( out, "FG %c\n", fg );
            }
            break;

            case 'b':/*Background*/
            if (command->data.integer != bg)
            {
                bg = command->data.integer;
                fprintf( out, "BG %c\n", bg );
            }
            break;
            #endif

            case 'd':/* This is the big one, where stuff happens! Draw */
            if (list->head->next != NULL)
            {
                draw += command->data.real;
                if (((Command*)list->head->next->data)->type != 'd')
                {
                    fprintf( out, "DRAW %f\n", draw );
                    draw = 0.0;
                }
            }
            else
            {
                draw += command->data.real;
                fprintf( out, "DRAW %f\n", draw );
            }
            break;
        }
        removeFirst( list );/*Remove executed command*/
    }
    return 0;
}
