#!/bin/sh

# Small and ugly "dynamic compiler" for Raydium apps, with ODE
# Use "./odyncomp.sh test6.c" for example.

ulimit -c 0
make

if [ "$?" != "0" ]; then
    echo "build failed"
    exit 1
fi

if [ -z "$CC" ]; then
    CC="gcc";
fi

if [ -n "$AR_PATH" ]; then
    if [ ! -d "$AR_PATH" ]; then
	echo "Unable to find ARToolKit (AR_PATH env)"
	exit 1
    fi
    AR_PATH_INCLUDE="-I $AR_PATH/include"
    AR_PATH_LIBS="$AR_PATH/lib/libAR.a"
fi

rm test
$CC $1 -g -Wall -DFORCE_LIBRAYDIUM -o test libraydium.so \
-Iraydium/php/ -Iraydium/php/main/ -Iraydium/php/Zend -Iraydium/php/TSRM -Iraydium/ode/include/ $AR_PATH_INCLUDE $AR_PATH_LIBS
sync
export LD_LIBRARY_PATH=.
shift
./test "$@"
