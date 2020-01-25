/*****************************************************************************
* File:      FileInput.c                                                     *
* Author:    Sean Ashton * Student ID:                               *
* Reference: None.                                                           *
* Unit:      COMP1000 Unix and C Programming                                 *
* Purpose:   Read in a file and populate a linked list                       *
* Comments:  Valid command must be exactly a command name (case insensitive) *
* Requires:  LinkedList.h, stdio.h, stdlib.h, string.h, ctype.h              *
* Created:   04/10/2018 * Last Modified: 13/10/2018                          *
*****************************************************************************/
#include "FileInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define VALID_LEN 6
#define COMMAND_LEN 51

/* Forward Declarations */
void sToLower( char* );
int validCommand( char* );
void initComStruct( Command* comSruct, char comChar );

/* Array of valid commands to check against */
const char* const validCommands[VALID_LEN] = {"rotate", "move", "draw", "fg",
                                                             "bg", "pattern"};

/*****************************************************************************
* Name: readLines
* Purpose: read in a file to a linked list of appropriate structs
* Comments: This function is somewhat long, however due to doing a lot of
*           small tasks readability would not be improved by creating a
            function to further modularise the functionality. Extendability
            could be enhanced by break the switch statement out to a separate
            function.
* Imports: fileName, the name of the file to read in.
*           list, a linked list to store all the commands in
* Exports: returnCode, the error code if something goes wrong
*****************************************************************************/
int readLines( char *fileName, LinkedList *list )
{
    FILE *in;
    int done = FALSE;
    int nRead;
    /* 0: Success, 1: NULL filename, 2: Failed to open file,
        3: Invalid format, 4: Error with inserting to list,
        5: No valid commands */
    int returnCode = 0;
    char command[COMMAND_LEN];
    char comChar;
    char *token;
    Command *comStruct;

    if ( fileName == NULL )
    {
        returnCode = 1;
    }
    else
    {
        in = fopen( fileName, "r" );
        if ( in == NULL )
        {
            returnCode = 2;
        }
        else if ( ferror(in) )
        {
            perror( "File read error\n" );
        }
        else
        {
            do
            {
                /* Check if end of file */
                if ( fgets( command, COMMAND_LEN, in ) == NULL )
                {
                    done = TRUE;
                }
                else
                {
                    token = strtok( command, " " );/*Get and check command*/
                    if ( validCommand( token ) )/*Converts to lowercase as a
                                                            side effect */
                    {
                        /* If the command is valid, use the first letter to
                           switch between cases, as it is unique per command*/
                        comChar = ( token[0] );
                        token = strtok( NULL, " " );/*Get the data*/
                        comStruct = (Command*)malloc( sizeof( Command ) );
                        /* Initialise Command Struct */
                        initComStruct( comStruct, comChar );
                        switch ( comChar )
                        {
                            /* In case of real number, also accept angles
                              greater than 360 degrees, will normalise later*/
                            case 'r': case 'm': case 'd':
                            nRead = sscanf( token, "%lf",
                                                    &comStruct->data.real );
                            break;

                            /* In case of integer, will ignore invalid lines
                                later. */
                            case 'f': case 'b':
                            nRead = sscanf( token, "%d",
                                                &comStruct->data.integer );
                            break;

                            /* In case of pattern, store char */
                            case 'p':
                            nRead = sscanf( token, " %c",
                                                &comStruct->data.pattern );
                            break;

                            default:
                            nRead = 0;
                        }
                        if ( nRead != 1 )/*nRead is 1 if the command is okay*/
                        {
                            done = TRUE;
                            /* If the command is invalid, free the memory */
                            free( comStruct );
                            returnCode = 3;
                        }/* Attempt to insert comStruct into list,
                            handle the case that insert fails */
                        else if ( insertLast( list, comStruct ) != 0 )
                        {
                            /* If the insert fails, free the memory */
                            free( comStruct );
                            returnCode = 4;
                        }
                    }
                    else
                    { /* If the command is not on the valid list */
                        #ifdef VERBOSE
                        fprintf( stderr, "Invalid Command: %s\n", token );
                        #endif
                        returnCode = 3;
                        done = TRUE;
                    }
                }
            }
            while ( !done );
            fclose( in );
            if ( !returnCode && list->head == NULL )
            {
                returnCode = 5;
            }
        }
    }
    return returnCode;
}

/*****************************************************************************
* Name: validCommand
* Purpose: Check if a given command is valid
* Imports: char*, String representing the command
* Exports: int, result of validity check
*****************************************************************************/
int validCommand( char *command )
{
    int ii;
    int ret = FALSE;
    sToLower( command );/* Allow for case insensitivity */
    for (ii = 0; ii < VALID_LEN; ii++)
    {
        if ( strcmp( command, validCommands[ii] ) == 0 )
        {
            ret = TRUE;
        }
    }
    #ifdef VERBOSE
    fprintf( stderr, "Invalid command in validCommand: %s", command );
    #endif
    return ret;
}

/*****************************************************************************
* Name: sToLower
* Purpose: Convert a string to lowercase
* Imports: char*, String to convert
*****************************************************************************/
void sToLower( char *string )
{
    int ii, len;
    len = strlen( string );
    for ( ii = 0; ii < len; ii++ )
    {
        string[ii] = (char)tolower( string[ii] );
    }
}

/*****************************************************************************
* Name: readError
* Purpose: Present an error message to the user in case of file read error.
* Import: int error, error code from FileInput.c
*****************************************************************************/
void readError( int error )
{
    switch ( error )
    {
        case 1:
        fprintf( stderr,"Error with code. NULL file. Return to programmer.\n"
                                                                            );
        break;

        case 2:
        fprintf( stderr, "Error: Failed to open input file," );
        fprintf( stderr, " please check the name and try again\n" );
        break;

        case 3:
        fprintf( stderr, "Error: File has invalid formatting.\n" );
        break;

        case 4:
        fprintf( stderr,"Error with list insert. Return to programmer.\n" );
        break;

        case 5:
        fprintf( stderr, "Error: File has no valid commands.\n" );
        break;

        default:
        fprintf( stderr, "Unknown error. What did you do?\n" );
    }
}

/*****************************************************************************
* Name: initComStruct
* Purpose: Initialise a comStruct struct
* Imports: comStruct, comChar: the command type being initialised
*****************************************************************************/
void initComStruct( Command *comStruct, char comChar )
{
    comStruct->type = comChar;
    comStruct->data.real = 0.0;
}
