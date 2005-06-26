#!/bin/sh

# This script will build a release archive of Raydium

APPS="test6.c raydium_modler.c willou.c willt5.c train.c
train.php skydiver.c score.php playlist.php cam.c tests.c
Makefile configure
comp.sh ocomp.sh ostatcomp.sh odyncomp.sh
"

DIRS="raydium rayphp"


if [ ! -d "build" ]; then
    echo "creating build directory"
    mkdir build
fi


# create names
file="raydium-`date +%Y-%m-%d`"
directory="build/$file"

if [ -d "$directory" ]; then
    echo "$directory already exists !"
    exit 1
fi

echo "creating $directory ..."
mkdir "$directory"

if [ "$?" != "0" ]; then
    echo "error creating $directory"
    exit 1
fi

# cleaning Raydium
cd ..
make clean_f
cd -

# copy app files
for i in $APPS; do
    echo "file $i"
    cp "../$i" "$directory"
done

# copy dirs
for i in $DIRS; do
    echo "directory $i"
    cp -Rp "../$i" "$directory"
done

# regenerate "dist"
echo "creating a new dist directory ..."
mkdir "$directory/dist"

if [ "$?" != "0" ]; then
    echo "error creating $directory/dist"
    exit 1
fi

for i in `ls`; do
    if [ -f "$i" ]; then
	echo "dist file $i"
	cp "$i" "$directory/dist"
    fi
done

# build archive
cd build
tar czf "$file.tar.gz" "$file"

if [ "$?" != "0" ]; then
    echo "error creating tar.gz archive"
    exit 1
fi

echo "archive is ready in build directory"
