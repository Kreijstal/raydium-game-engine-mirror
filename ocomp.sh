#!/bin/sh

# Small and ugly "static compiler" for Raydium apps, with ODE
# Use "./ocomp.sh test6.c" for example.

ulimit -c 0
rm test
gcc -g $1 -Wall -o test -L/usr/X11R6/lib/ -lGL -lglut -lGLU -lm -lopenal -ljpeg -Iode/include/ ode/lib/libode.a -lvorbis -lvorbisfile -logg -Iphp/ -Iphp/include -Iphp/main/ -Iphp/Zend -Iphp/TSRM php/libs/libphp4.a -lresolv -lcrypt -lz
sync
./test $2 $3 $4 $5 $6
