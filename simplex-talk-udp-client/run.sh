#!/bin/bash

for i in 1 2 3 4 5 6 7 8 9 10
do
  echo "--------------------------------------------------------" >> log.txt
  echo "Starting client $i" >> log.txt
  ./Debug/simplex-talk-udp-client 127.0.0.1 < input/c$i.txt 2&>> log.txt &
done 
