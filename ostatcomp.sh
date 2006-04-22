#!/bin/sh

# Small and ugly "full static compiler" for Raydium apps, with ODE
# Use "./ostatcomp.sh test6.c" for example.

ulimit -c 0
rm test
gcc -g $1 -Wall -o test -Wl,-dynamic-linker=/lib/ld-linux.so.2 -Wl,-Bstatic -L/usr/X11R6/lib/ \
-lm -lXi -lstdc++ -ljpeg \
-Iraydium/ode/include/ raydium/ode/lib/libode.a \
-lXmu -lXinerama -lopenal -lalut -lvorbisfile -lvorbis -logg \
-Iraydium/php/ -Iraydium/php/main/ -Iraydium/php/Zend -Iraydium/php/TSRM raydium/php/libs/libphp5.a \
-lcrypt -lssl -lresolv -lz -lcurl -lxml2 \
-Wl,-Bdynamic -lGL -lGLU -lasound -lpthread
sync
shift
./test "$@"
