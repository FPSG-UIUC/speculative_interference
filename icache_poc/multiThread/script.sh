#!/bin/bash

val=(8750 10150 11000 11625 12375 13675 14425 15350 16500 18450 19800 22775 25500 29000 32750 39800 50600 66000 102225 150000 198800 250000 300000 500000 650000 1012250)
#val=(8750 10150 11000 11625 12375 13675 14425 15350 16500)
make
for loop in ${val[@]}
do
  #echo $i
  #loop=$val[$i]
  to=$(expr $loop / 1200)
  echo "Running with training loop ${loop}"
  echo $to
  count=0
  fail=0
  while [ $count -lt 3 ]
  do
    timeout $to ./multiThreadIC C $loop
    if [ $? -eq 124 ]; then
      fail=$[$fail+1]
      if [ $fail -ge 3 ]; then
        echo 'Error'
        count=3
      fi
      sleep 5
    else
      echo $count
      count=$[$count+1]
    fi
    sleep 9
  done
  sleep 1
done
