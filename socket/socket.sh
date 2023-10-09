#!bin/bash
sudo echo "sudo"
input=$1
if [ $input == "send" ]; then
    gcc -o $input sender.c
    ./$input
elif [ $input == "recv" ]; then
    gcc -o $input receiver.c
    ./$input
else
    echo "error"
fi