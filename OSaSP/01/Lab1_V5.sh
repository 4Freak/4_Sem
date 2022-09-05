#!/bin/bash

Check=1
if [[ -n "$1" ]] && [[ -n "$2" ]] && [[ -n "$3" ]]; then

        for i in "$1" "$2" "$3"
        do
                for j in "|" "<" ">" ":" "\"" "?" "*" "|" "\\"
                do
                        if [[ "$i" == *"$j"* ]]; then
                                Check=0
                                { echo Invalid parametr;
                                echo Parametr "\"""$i""\"" contain "\"""$j""\" "; } >&2
			fi
                done
        done
	if [[ ! -d "$2" ]] ;
	then
		Check=0
    		{ echo "Directory $2 does not exists"; } >&2
	fi
else
        { echo Unexpected number of parametrs;
	echo Parametrs:;
	echo First - Username;
	echo Second - Directory;
	echo Third - Output file name; }  >&2
	Check=0
fi

if [[ "$Check" == 1 ]]; then
find "$2" -type f -user "$1" -printf "File path: " -exec realpath {} \; \
 			     -printf "File name: " -printf "%f\n" \
			     -printf "File size: %s \n" >"$3"
echo -n "Files checked: "
find "$2" -type f | wc -l
fi
