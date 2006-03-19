#!/bin/sh

# Small and ugly "static compiler" for Raydium apps, without ODE
# Use "./comp.sh raydium_modler.c" for example.

ulimit -c 0
rm test
gcc $1 -Wall -o test -L/usr/X11R6/lib/ -lXinerama -lGL -lGLU -lm -lopenal -lalut -lvorbis -logg -lvorbisfile -ljpeg -Iphp/ -Iphp/main/ -Iphp/Zend -Iphp/TSRM php/libs/libphp4.a -lresolv -lcrypt -lz
sync
shift
./test "$@"
