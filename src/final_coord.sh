#!/bin/bash

echo "Usage: final_coord.sh path_to_out natom"

natom=$2
nline=$(echo "$natom + 10" | bc -l)
output=final_coord
tmpout=$output.tmp

grep -A $nline "Begin final coordinates" $1  > $tmpout
sed -i "s/(//g" $tmpout
sed -i "s/)//g" $tmpout
alat=$(awk 'NR==4 {print $3}' $tmpout)

declare array
array[0]=$(awk 'NR==5''{print $1}' $tmpout)
array[1]=$(awk 'NR==5''{print $2}' $tmpout)
array[2]=$(awk 'NR==5''{print $3}' $tmpout)
array[3]=$(awk 'NR==6''{print $1}' $tmpout)
array[4]=$(awk 'NR==6''{print $2}' $tmpout)
array[5]=$(awk 'NR==6''{print $3}' $tmpout)
array[6]=$(awk 'NR==7''{print $1}' $tmpout)
array[7]=$(awk 'NR==7''{print $2}' $tmpout)
array[8]=$(awk 'NR==7''{print $3}' $tmpout)

for i in `seq 0 8`
do
  array[$i]=$(echo "${array[$i]} * $alat" | bc -l)
done

echo "CELL_PARAMETERS bohr" > $output
echo ${array[0]} ${array[1]} ${array[2]} >> $output
echo ${array[3]} ${array[4]} ${array[5]} >> $output
echo ${array[6]} ${array[7]} ${array[8]} >> $output
echo >> $output

tail -$(echo "3+$natom" | bc -l) $tmpout | head -$(echo "1+$natom" | bc -l) >>$output

rm -f $tmpout
