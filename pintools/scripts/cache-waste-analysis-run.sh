#!/bin/sh

#
# Requires: A memtracker trace file
# Executes: cache-waste-analysis using the provided MEMTRACKER_TRACE
# Outputs: cache-waste-analysis trace to OUTFILE
# 

MEMTRACKER_TRACE=memtracker-log.txt
CACHE_WASTE_ANALYSIS=../analysis-tools/cache-waste-analysis
OUTFILE=cache-waste-analysis-log.txt

sudo ./$CACHE_WASTE_ANALYSIS -f $MEMTRACKER_TRACE > $OUTFILE
