#!/bin/bash

if [ $# -eq 3 ]
then
	if [ -f $3 ]
	then
		if [ $1 == "-r" ]
		then
			if [ $2 == "-u" ]
			then
				name=$3
				uname=${name^^}
				if [ $name == $uname ]
				then
					echo " The name's file is already in upper case"
				else
					mv $name $uname
				fi
			else
				if [ $2 == "-l" ]
				then
					name=$3
					lname=${name,,}
					if [ $name == $lname ]
					then
						echo " The name's file is already in lower case "
					else
						mv $name $lname
					fi
				else
					echo " Second argument incorrect "
				fi
			fi
		else
			echo "First argument incorrect"
		fi
	else
		echo "File doesn't exist"
fi

else
	for var in "$@"
	if [ var !=  "-r"] || [var !=  "-u" ] || [var != "-l"];
	do
		if [ $2 == "-u" ]
			then
				name=$var
				uname=${name^^}
				if [ $name == $uname ]
					then
					echo " The name's file is already in upper case"
				else
					mv $name $uname
				fi
		else 
			if [ $2 == "-l" ]
			then
				name=$var
				lname=${name,,}
				if [ $name == $lname ]
					then
						echo " The name's file is already in lower case "
					else
						mv $name $lname
				fi	
			fi
		fi
		fi
	done
fi



