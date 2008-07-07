HEADERS=../raydium/headers
GEANYLOCAL=~/.geany/tags/
geany -g -P raydium-tags-tmp.c.tags $HEADERS/*.h
cp raydium-tags-tmp.c.tags $GEANYLOCAL/raydium.c.tags

