#!/bin/bash



arrayLength=( 16 32 64 128 256 ) # 512 1024 )

echo "run simulations with saw"
for myN in "${arrayLength[@]}"; do
    filename="saw_${myN}"
    echo $myN
    while [ ! -s "${filename}_r.dat" ]; do
        if [ -s "${filename}.bfm" ]; then rm ${filename}.bfm; fi
        #time -p -o ${filename}_time.dat ./selfAvoidingWalker -o ${filename}.bfm -n $myN -r ${filename}_r.dat > ${filename}.out
        ./selfAvoidingWalker -o ${filename}.bfm -n $myN -r ${filename}_r.dat
    done
done
echo "simulations done"

echo "run scattering function analyzer"
for myN in "${arrayLength[@]}"; do
    filename="saw_${myN}"
    ./analyzeScattering -i ${filename}.bfm -o ${filename}_scat.dat
    #if [ ! -s "${filename}_scat.dat" ]; do
    #    ./analyzeScattering -i ${filename}.bfm -o ${filename}_scat.dat
    #done
done
