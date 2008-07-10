#!/bin/sh

# Small and ugly "dynamic compiler" for Raydium apps, with ODE
# Use "./odyncomp.sh test6.c" for example.

# When you need special dependencies (should work, but not supported !)
# you may define following env vars:

# AR_PATH
#       for AR Toolkit (http://artoolkit.sourceforge.net/)

# LIBWII_INCLUDES
#       for libcwiimote (http://libwiimote.sourceforge.net)

# ODYNCOMP_FLAGS
#       for anything else you may need ...

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

# Wiimote(s) support
# http://libwiimote.sourceforge.net/
if [ -n "$LIBWII_INCLUDES" ]; then
    if [ ! -f "$LIBWII_INCLUDES/wiimote.h" ]; then
        echo "Unable to find libwiimote includes (LIBWII_INCLUDES env)"
        exit 1
    fi
    LIBWII_PATH_INCLUDE="-I $LIBWII_INCLUDES"
    LIBWII_ADDS="-D_ENABLE_TILT -D_ENABLE_FORCE -lbluetooth -lcwiimote"
fi

if [ -f test ]; then
    rm test
fi
$CC $1 -g -Wall -DFORCE_LIBRAYDIUM -o test libraydium.so \
-Iraydium/php/ -Iraydium/php/main/ -Iraydium/php/Zend -Iraydium/php/TSRM \
-Iraydium/ode/include/ $AR_PATH_INCLUDE $AR_PATH_LIBS $LIBWII_PATH_INCLUDE $LIBWII_ADDS $ODYNCOMP_FLAGS
export LD_LIBRARY_PATH=.
shift
./test "$@"
