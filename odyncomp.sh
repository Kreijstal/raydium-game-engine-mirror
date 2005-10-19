#!/bin/sh

# Small and ugly "dynamic compiler" for Raydium apps, with ODE
# Use "./odyncomp.sh test6.c" for example.

ulimit -c 0
make

if [ "$?" != "0" ]; then
    echo "build failed"
    exit 1
fi

rm test
gcc $1 -g -Wall -DFORCE_LIBRAYDIUM -o test libraydium.so -Iphp/ -Iphp/main/ -Iphp/Zend -Iphp/TSRM -Iode/include/
sync
export LD_LIBRARY_PATH=.
./test $2 $3 $4 $5 $6
