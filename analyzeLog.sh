#!/bin/bash

checks=$(grep "Check" $1 | wc -l)  

echo "$checks checks  made in $1"



