#!/bin/bash

checks=$(grep "Check" $1 | wc -l)  
echo "$checks checks  made in $1"


totalSleep=$(grep "Sleep" $1 | awk '{ SUM += $NF} END { print SUM }')
echo "$totalSleep sleep time in $1"


numWrites=$(grep "Write" $1 | wc -l)
echo "$numWrites writes in $1"

numReads=$(grep "Reads" $1 | wc -l)
echo "$numReads reads in $1"
