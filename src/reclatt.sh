#!/bin/bash

echo "usage: reclatt path_to_out pw/vasp"

pi=3.14159265359

if [ "$2" = "pw" ]; then
  alat=$(grep "lattice parameter (alat)" $1 | head -1 | awk '{print $5}')
  echo $(echo "2*$pi/$alat" | bc -l) > reclatt

  grep -A3 "reciprocal axes" $1 | tail -3 >> reclatt
  sed -i "s/               b(1) = (//g" reclatt
  sed -i "s/               b(2) = (//g" reclatt
  sed -i "s/               b(3) = (//g" reclatt
  sed -i "s/)//g" reclatt

  elif [ "$2" = "vasp" ]; then
  scale=1.8897261
  echo "lattice constant is set to be 1.0/a0 in general for vasp"
  echo "a0 is bohr radius"

  grep -A 3 "reciprocal lattice vectors" $1 |tail -3 > reclatt

  declare array
  array[0]=$(awk 'NR==1''{print $4}' reclatt)
  array[1]=$(awk 'NR==1''{print $5}' reclatt)
  array[2]=$(awk 'NR==1''{print $6}' reclatt)
  array[3]=$(awk 'NR==2''{print $4}' reclatt)
  array[4]=$(awk 'NR==2''{print $5}' reclatt)
  array[5]=$(awk 'NR==2''{print $6}' reclatt)
  array[6]=$(awk 'NR==3''{print $4}' reclatt)
  array[7]=$(awk 'NR==3''{print $5}' reclatt)
  array[8]=$(awk 'NR==3''{print $6}' reclatt)

  echo $(echo "2*$pi/$scale" | bc -l) > reclatt

  echo ${array[0]} ${array[1]} ${array[2]} >> reclatt
  echo ${array[3]} ${array[4]} ${array[5]} >> reclatt
  echo ${array[6]} ${array[7]} ${array[8]} >> reclatt

  else
  echo "Please assign pw/vasp"
fi

cat reclatt
