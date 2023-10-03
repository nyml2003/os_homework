#!bin/bash
gcc -o pressure pressure.c -lpthread
for i in {1..100}
do
    echo "Test $i"
    read_num=$(($RANDOM%5000+5000))
    ./pressure 100 $read_num
done