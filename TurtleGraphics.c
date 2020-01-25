/*****************************************************************************
* File:      TurtleGraphics.c                                                *
* Author:    Sean Ashton * Student ID:                               *
* Unit:      COMP1000 Unix and C Programming                                 *
* Purpose:   Draw pretty pictures                                            *
* Reference: None.                                                           *
* Comments:  Continue on a many errors as possible.                          *
* Requires:  LinkedList.h, effect.h, FileInput.h, stdio.h, stdlib.h, math.h  *
* Created:   04/10/2018 * Last Modified: 13/10/2018                          *
*****************************************************************************/
#include "TurtleGraphics.h"
#include "LinkedList.h"
#include "effects.h"
#include "FileInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Constants */
#define M_PI 3.14159265358979323846
#define REAL_DEFAULT 0.0
#ifdef SIMPLE
#define FG_DEFAULT 0 /*As per update from Amy posted 13 Oct 2018*/
#define BG_DEFAULT 7  /*As per update from Amy, though 7 not 15
                                                 posted 13 Oct 2018*/
#else
#define FG_DEFAULT 7 /*Per the spec "white"*/
#define BG_DEFAULT 0 /*Per the spec, black*/
#endif
#define FG_MAX 15
#define BG_MAX 7
#define PATT_DEFAULT '+'
#define LOGFILE "graphics.log"
#define LOGSTR_LEN 101

/* Forward declarations */
void graphicsLog( char *string );
void plotterFunc( void *plotData );
int plot( LinkedList *list );
/* Utility functions for exceuting commands */
void rotate( double *pang, Command *command );
void move( double *px, double *py, double *pang, Command *command );
/* Omitting pattern as it's 1 line. */
void foreground( int *pfg, Command *command );
void background( int *pbg, Command *command );
void draw( double *px, double *py, double *pang, char *ppatt,
                                                        Command *command );

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
            graphicsLog( "---" );
            clearScreen(  );
            setFgColour( FG_DEFAULT );
            setBgColour( BG_DEFAULT );
            plot( list );
            graphicsLog(NULL);
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

int plot( LinkedList *list )
{
    /* Variable declarations and initialisation */
    double x, y, ang;
    char patt;
    #ifndef SIMPLE
    int fg, bg;
    #endif
    Command *command;
    x = y = ang = REAL_DEFAULT;
    patt = PATT_DEFAULT;
    #ifndef SIMPLE
    fg = FG_DEFAULT;
    bg = BG_DEFAULT;
    #endif
    #ifdef VERBOSE
    graphicsLog( "VERBOSE Mode" );
    #endif

    while ( list->head != NULL )
    {
        command = (Command*)list->head->data;
        switch ( command->type )
        {
            case 'r':/*Rotate*/
            rotate( &ang, command );
            break;

            case 'm':/*Move*/
            move( &x, &y, &ang, command );
            break;

            case 'p':/*Pattern*/
            patt = command->data.pattern;
            break;

            #ifndef SIMPLE
            case 'f':/*Foreground*/
            foreground( &fg, command );
            break;

            case 'b':/*Background*/
            background( &bg, command );
            break;
            #endif

            case 'd':/* This is the big one, where stuff happens! Draw */
            draw( &x, &y, &ang, &patt, command );
            break;
        }
        removeFirst( list );/*Remove executed command*/
    }
    penDown(  );

    return 0;
}

/*****************************************************************************
* Name: plotterFunc
* Purpose: Print a character when called
* Imports: void*, data to pass for what to print
*****************************************************************************/
void plotterFunc( void *plotData )
{
    PlotData *inData = (PlotData*)plotData;
    if ( inData->length )/*Plot only as many points as case d: asks for.*/
    {
        inData->length--;
        printf( "%c",*(inData->ch) );
    }
}

/*****************************************************************************
* Name: graphicsLog
* Purpose: append command to log file
* Imports: char*, string to write out.
*****************************************************************************/
void graphicsLog( char *string )
{
    static FILE *out = NULL;
    /*Keep output open for the duration of the program*/
    if (out == NULL)
    {
        out = fopen( LOGFILE, "a" );
    }
    fprintf( out, "%s\n", string );
    #ifdef DEBUG
    fprintf( stderr, "%s\n", string );
    #endif
    if (out != NULL && string == NULL)/*Use a NULL input to trigger close*/
    {
        fclose( out );
    }
}

/*****************************************************************************
* Name: rotate
* Purpose: rotate the current angle and keep within 0-360 degrees.
* Imports: pang, pointer to current angle; command, command to execute
*****************************************************************************/
void rotate( double* pang, Command* command )
{
    #ifdef VERBOSE
    double dTmp = *pang;
    char logStr[LOGSTR_LEN];
    #endif
    *pang += command->data.real;
    /* normalise angle to within 0 - 360 degree */
    while ( *pang > 360.0 )
    {
        *pang -= 360.0;
    }
    while ( *pang < 0 )
    {
        *pang += 360.0;
    }
    #ifdef VERBOSE
    sprintf( logStr, "\tROT  (%6.3f)-(%6.3f)", dTmp, *pang );
    graphicsLog( logStr );
    #endif
}

/*****************************************************************************
* Name: move
* Purpose: move the cursor without drawing anything.
* Imports: px py, pointers to current position; command, command to execute
*****************************************************************************/
void move( double *px, double *py, double *pang, Command *command )
{
    char logStr[LOGSTR_LEN];
    double oldX = *px;
    double oldY = *py;
    *px += ( command->data.real ) * cos( (*pang) * ( M_PI / 180.0 ) );
    *py -= ( command->data.real ) * sin( (*pang) * ( M_PI / 180.0 ) );
    /* Keep coordinates within bounds, not sure with maximums */
    if ( *px < 0.0 ) { *px = 0.0; }
    if ( *py < 0.0 ) { *py = 0.0; }
    sprintf( logStr, "\tMOVE (%6.3f,%6.3f)-(%6.3f,%6.3f)", oldX, oldY,
                                                            *px, *py );
    graphicsLog( logStr );
}

/*****************************************************************************
* Name: foreground
* Purpose: change the foreground colour, checking bounds.
* Imports: pfg, pointer to current colour; command, command to execute
*****************************************************************************/
void foreground( int *pfg, Command *command )
{
    int tmp = command->data.integer;
    /* Ignore invalid colour input, no reason to exit. */
    if ( ( tmp >= 0 ) && ( tmp <= FG_MAX ) )
    {
        *pfg = tmp;
        setFgColour( *pfg );
    }
}

/*****************************************************************************
* Name: background
* Purpose: change the background colour, checking bounds.
* Imports: pbg, pointer to current colour; command, command to execute
*****************************************************************************/
void background( int* pbg, Command* command )
{
    int tmp = command->data.integer;
    /* Ignore invalid colour input, no reason to exit. */
    if ( ( tmp >= 0 ) && ( tmp <= BG_MAX ) )
    {
        *pbg = tmp;
        setBgColour( *pbg );
    }
}

/*****************************************************************************
* Name: draw
* Purpose: draw a line to new x and y.
* Imports: px py, pointer to current location; command, command to execute
*****************************************************************************/
void draw( double *px, double *py, double *pang, char *ppatt,
                                                            Command *command )
{
    char logStr[LOGSTR_LEN];
    double oldX = *px;
    double oldY = *py;
    PlotData pData;
    *px += (command->data.real) * cos( (*pang) * ( M_PI / 180.0 ) );
    *py -= (command->data.real) * sin( (*pang) * ( M_PI / 180.0 ) );
    /* Keep coordinates within bounds, not sure with maximums */
    if ( *px < 0.0 ) { *px = 0.0; }
    if ( *py < 0.0 ) { *py = 0.0; }
    sprintf( logStr, "\tDRAW (%6.3f,%6.3f)-(%6.3f,%6.3f)", oldX, oldY,
                                                                *px, *py );
    graphicsLog( logStr );
    /* Add 0.5 to effectively round to the nearest number,
        with the exception of x, as the plot function is wierd */
    /* effects.c is broken and plots an extra point at the end
        of each plot command. Added a counter so that plotterFunc
        only prints as many points as the draw call asks for */
    pData.length = (int)(command->data.real + 0.5);
    pData.ch = ppatt;
    line( (int)(oldX + 0.5), (int)(oldY + 0.5),
            (int)( *px-cos( (*pang) * ( M_PI / 180.0 ) ) + 0.5),
                (int)( *py+sin( (*pang) * ( M_PI / 180.0 ) )+0.5),
                    &plotterFunc, (void*)&pData );
}
