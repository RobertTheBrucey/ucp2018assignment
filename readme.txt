Sean Ashton UCP Assignment readme

Date created: 04/10/2018

Date last modified: 13/10/2018

Purpose: Draw terminal based "turtle" graphics

Files in project:
FileInput.c - Used to import a file to display
FileInput.h - header for FileInput.c
LinkedList.c - used to define and interact with Linked Lists
LinkedList.h - header for LinkedList.c
Makefile - make file for this project, includes the following additional targets:
    TurtleGraphicsSimple - prints in black and white
    TurtleGraphicsDebug - prints log entries to stderr as well as graphics.log
    TurtleGraphicsVerbose - adds rotate to logging
    TurtleGraphicsMegaDebug - the same as Debug and Verbose and adds the -g flag
TurtleGraphics.c - The main program logic
TurtleGraphics.h - header for TurtleGraphics.c
effects.c - provided functions for terminal display
effects.h - header file for effects.c
readme.txt - This file

Test Files:
valgrindTest.sh - runs all the test data files with valgrind to check for memory issues.
TestData/
    angledPartial.txt - Tests drawing an angled (45 degree) line
    asfasf.rtf - Tests for invalid file type, this caused a segfault due to a missing exit condition
    charizard.txt - provided test file, displays a pokemon
    fullTest.txt - tests all functionality in a single file.
    test.mp3 - another invalid file type test, triggered the same fault as asfasf.rtf
    testBadDataType.txt - tests incorrent data type in a command
    testBadNumOfArgs.txt - tests with excessive arguments per command
    testBlankLine.txt - tests blank line in input file handling
    testBoxSpiral.txt - Draws a simple spiral pattern, though it's boxy.
    testDraw1.txt - Draws a single point, used for debugging double printing (was length+1 printing)
    testEmpty.txt - Empty file
    testInvalidCommand.txt - contains invalid commands
    testLine.txt - draws a line
    testShallowAng - draws a 1 degree line to check for rounding errors.

Functionality:
Accepts a single command line parameter (and input file) and draws graphics based on
    6 commands:
    FG: foreground colour from 0 to 15
    BG: background colour from 0 to 7
    PATTERN: set the character to print
    ROTATE: rotate the direction of travel specified in degrees
    MOVE: move the cursor by an amount in the current direction
    DRAW: move and print in the current direction
Outputs MOVE and DRAW commands to graphics.log, otherwise just displays the graphic and exits

Known bugs

Additional functionality:
Contains addition make targets for debugging purposes, described above
