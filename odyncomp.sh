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
gcc $1 -g -Wall -DFORCE_LIBRAYDIUM -o test libraydium.so \
-Iraydium/php/ -Iraydium/php/main/ -Iraydium/php/Zend -Iraydium/php/TSRM -Iraydium/ode/include/
sync
export LD_LIBRARY_PATH=.
shift
./test "$@"
