ulimit -c 0
make
rm test
gcc $1 -g -Wall -DFORCE_LIBRAYDIUM -o test libraydium.so -Iphp/ -Iphp/main/ -Iphp/Zend -Iphp/TSRM -Iode/include/
sync
export LD_LIBRARY_PATH=.
./test $2 $3 $4 $5 $6
