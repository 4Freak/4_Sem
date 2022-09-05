#!/bin/bash

IFS=$'\n'
Check=1
if [[ -n "$1" ]] && [[ -n "$2" ]]; then

	for i in "$1" "$2" "$3"
	do
		for j in "|" "<" ">" ":" "\"" "?" "*" "|" "\\"
		do
			if [[ "$i" == *"$j"* ]]; then
				check=0
				{ echo Invalid parametr; 
				echo Parametr "\"""$i""\"" contain "\"""$j""\""; } >&2
			fi
		done
	done

        if [[ ! -d "$2" ]] ;
        then
                { echo "Directory $2 does not exists"; } >&2
                Check=0
        fi

else
	Check=0
        { echo Unexpected number of parametrs; 
	echo Paramerts:;
	echo First - output file name;
	echo Ssecond - start directory;
	echo Third - extension without symbol "."; } >&2
fi

if [[ "$Check" = 1 ]]; then
FileName=$1
StartDir=$2
Extension=$3
find "$2" -type f -name "*.$Extension" -printf "%f\n" | sort -n >"$FileName"
fi
