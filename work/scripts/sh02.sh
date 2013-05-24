#!/bin/bash

#if [ "$1" == "hello" ]; then
#	echo "Hello, how are you ?"
#elif [ "$1" == "" ]; then
#	echo "You MUST input parameters, ex> {$0 someword}"
#else
#	echo "The only parameter is 'hello', ex> {$0 hello}"
#fi

if [ "$1" == "hello" ];then
	echo "hello,how are you!"
elif [ "$1" == "" ];then
	echo "you must input hello first/."
else
	echo "error"
fi
