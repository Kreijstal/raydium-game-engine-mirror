#!/bin/sh

# Small and ugly "static compiler" for Raydium apps, without ODE
# Use "./comp.sh raydium_modler.c" for example.

ulimit -c 0
rm test
gcc $1 -Wall -o test -L/usr/X11R6/lib/ -lGL -lglut -lGLU -lm -lopenal -lvorbis -logg -lvorbisfile -ljpeg -Iphp/ -Iphp/main/ -Iphp/Zend -Iphp/TSRM php/libs/libphp4.a -lresolv -lcrypt -lz
sync
./test $2 $3 $4 $5 $6
