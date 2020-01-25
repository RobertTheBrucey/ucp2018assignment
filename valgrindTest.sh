#!/bin/bash
cd TestData
for file in $( ls ); do
    valgrind ../TurtleGraphics file 2> >(grep 'ERROR SUMMARY' >> ../errors.txt)
done
