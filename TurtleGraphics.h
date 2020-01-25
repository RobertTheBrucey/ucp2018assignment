/*****************************************************************************
* File:      TurtleGraphics.h                                                *
* Author:    Sean Ashton * Student ID:                               *
* Unit:      COMP1000 Unix and C Programming                                 *
* Purpose:   Draw pretty pictures                                            *
* Reference: None.                                                           *
* Comments:  This comment block is the maximum width of 78 characters.       *
* Requires:  Nothing. List dependencies here                                 *
* Created:   04/10/2018 * Last Modified: 09/10/2018                          *
*****************************************************************************/
#ifndef TURTLEGRAPHICS_H
#define TURTLEGRAPHICS_H 1
/*PlotData is used to pass information through the plot() call, it is required
    because the plot() function always draws 1 more pixel than is required,
    this allows a length to be passed to plotterFunc() such that it stops
    printing once the intended line has been printed */
typedef struct PlotData {
    int length;
    char* ch;
} PlotData;
#endif
