#!/bin/sh
row=$1
col=$2

if [ $# -lt 2 ] || [ $# -gt 2 ]
then
    echo "Invalid input"
    exit 0
fi

if ! [[ $row =~ ^[\-0-9]+$ ]] || ! [[ $col =~ ^[\-0-9]+$ ]]
then
    echo "Invalid input"
    exit 0
fi

if [ $row -lt 1 ] || [ $col -lt 1 ]
then
    echo "Input must be greater than 0"
    exit 0
fi

for ((i=1; i<=row; i++))
do
    for ((j=1; j<=col; j++))
    do
        printf "$i*$j=`expr $i \* $j` \t"
    done
    printf "\n"
done
exit 0
