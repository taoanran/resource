#!/bin/bash

echo "read a line of mk-ics.sh"
# read a file ++++++++++++++++++++++++++++++++
FILE=./mk-ics.sh

{
    cat $FILE | while read line
        do
            echo $line
        done
} 
#---------------------------------------------
