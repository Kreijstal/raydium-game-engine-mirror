#!/bin/sh

# Small and ugly "full static compiler" for Raydium apps, with ODE
# Use "./ostatcomp.sh test6.c" for example.

ulimit -c 0
rm test
gcc -g $1 -Wall -o test -Wl,-dynamic-linker=/lib/ld-linux.so.2 -Wl,-Bstatic -L/usr/X11R6/lib/ -lm -lXi -lstdc++ -ljpeg -Iode/include/ ode/lib/libode.a -lXmu -lXinerama -lopenal -lalut -lvorbisfile -lvorbis -logg -Iphp/ -Iphp/main/ -Iphp/Zend -Iphp/TSRM php/libs/libphp4.a -lresolv -lcrypt -lz -Wl,-Bdynamic -lGL -lGLU -lasound -lpthread
sync
./test $2 $3 $4 $5 $6
