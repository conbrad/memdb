#!/bin/sh

#
# Requires: PIN_ROOT environment variable set to extracted pin folder
# Executes: memtracker using the provided INPUT_PROGRAM 
# Outputs: memtracker trace to OUTFILE
# 

INPUT_PROGRAM=../test-programs/basic-pointer
MEMTRACKER=../memtracker/obj-intel64
OUTFILE=memtracker-log.txt

sudo $PIN_ROOT/pin.sh -t $MEMTRACKER/memtracker.so -- ./$INPUT_PROGRAM > $OUTFILE
