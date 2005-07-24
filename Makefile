## auto-generated Makefile
EXT_H = .h
ROOT = ./
RAYDIUM_DIR = raydium/
HEADERS_DIR = raydium/headers/
COMPILE_DIR = raydium/compile/
CC = gcc
AR = ar
RANLIB = ranlib
LOG_FILE = compile.log
SYSTEM_LIBS =  -lGL -lglut -lGLU -lm -lopenal -lvorbis -lvorbisfile -logg -lresolv -lcrypt -lz
OTHER_LIBS =  ode/lib/libode.a php/libs/libphp4.a
INCLUDE_PATH =  -Iode/include/ -Iphp/ -Iphp/include -Iphp/main/ -Iphp/Zend -Iphp/TSRM
LIBS_PATH =  -L/usr/X11R6/lib/

CFLAGS=-Wall
COMPILE_OPTIONS=-g
LDFLAGS=
LINKING_OPTIONS=-Wl,-soname,libraydium.so.0
AR_OPTIONS=

VER=0.0
VER1=0
VER2=0.0


default: all

all: libraydium.a libraydium.so
	@echo "Files created: $+"

help:
	@echo "Help compiling raydium"
	@echo "	make help	this"
	@echo "	make default	make all"
	@echo "	make all	create libraries"
	@echo "	make clean	remove compilation objects and libraries"
	@echo "	make clean_f	remove without confirmation"

################################################################################
# Create libraries
################################################################################


libraydium.so.0.0:  raydium/compile/background.o raydium/compile/callback.o raydium/compile/camera.o raydium/compile/capture.o raydium/compile/clear.o raydium/compile/console.o raydium/compile/file.o raydium/compile/fog.o raydium/compile/init.o raydium/compile/internal.o raydium/compile/joy.o raydium/compile/key.o raydium/compile/land.o raydium/compile/light.o raydium/compile/log.o raydium/compile/main.o raydium/compile/mouse.o raydium/compile/network.o raydium/compile/normal.o raydium/compile/object.o raydium/compile/ode.o raydium/compile/osd.o raydium/compile/parser.o raydium/compile/particle2.o raydium/compile/php.o raydium/compile/profile.o raydium/compile/random.o raydium/compile/rayphp.o raydium/compile/register.o raydium/compile/render.o raydium/compile/signal.o raydium/compile/sky.o raydium/compile/sound.o raydium/compile/texture.o raydium/compile/timecall.o raydium/compile/trigo.o raydium/compile/vertex.o raydium/compile/window.o raydium/compile/reg_api.o raydium/compile/gui.o raydium/compile/live.o
	@echo "Creating $@" >> $(LOG_FILE)
	gcc $(LDFLAGS) $(LINKING_OPTIONS) -o $@ -shared $+ $(LIBS_PATH) $(OTHER_LIBS) $(SYSTEM_LIBS)
	@echo "File created: $@"

libraydium.so.0: libraydium.so.0.0
	ln -s $< $@ || true

libraydium.so: libraydium.so.0
	ln -s $< $@ || true

libraydium.a.0.0:  raydium/compile/background.o raydium/compile/callback.o raydium/compile/camera.o raydium/compile/capture.o raydium/compile/clear.o raydium/compile/console.o raydium/compile/file.o raydium/compile/fog.o raydium/compile/init.o raydium/compile/internal.o raydium/compile/joy.o raydium/compile/key.o raydium/compile/land.o raydium/compile/light.o raydium/compile/log.o raydium/compile/main.o raydium/compile/mouse.o raydium/compile/network.o raydium/compile/normal.o raydium/compile/object.o raydium/compile/ode.o raydium/compile/osd.o raydium/compile/parser.o raydium/compile/particle2.o raydium/compile/php.o raydium/compile/profile.o raydium/compile/random.o raydium/compile/rayphp.o raydium/compile/register.o raydium/compile/render.o raydium/compile/signal.o raydium/compile/sky.o raydium/compile/sound.o raydium/compile/texture.o raydium/compile/timecall.o raydium/compile/trigo.o raydium/compile/vertex.o raydium/compile/window.o raydium/compile/reg_api.o raydium/compile/gui.o raydium/compile/live.o
	$(AR) $(AR_OPTIONS) -crv $@ $+
	$(RANLIB) $@ || true
	@echo "File created: $@"

libraydium.a.0: libraydium.a.0.0
	ln -s $< $@ || true

libraydium.a: libraydium.a.0
	ln -s $< $@ || true


################################################################################
# Other rules
################################################################################


headers:  raydium/headers/background.h raydium/headers/callback.h raydium/headers/camera.h raydium/headers/capture.h raydium/headers/clear.h raydium/headers/console.h raydium/headers/file.h raydium/headers/fog.h raydium/headers/init.h raydium/headers/internal.h raydium/headers/joy.h raydium/headers/key.h raydium/headers/land.h raydium/headers/light.h raydium/headers/log.h raydium/headers/main.h raydium/headers/mouse.h raydium/headers/network.h raydium/headers/normal.h raydium/headers/object.h raydium/headers/ode.h raydium/headers/osd.h raydium/headers/parser.h raydium/headers/particle2.h raydium/headers/php.h raydium/headers/profile.h raydium/headers/random.h raydium/headers/rayphp.h raydium/headers/register.h raydium/headers/render.h raydium/headers/signal.h raydium/headers/sky.h raydium/headers/sound.h raydium/headers/texture.h raydium/headers/timecall.h raydium/headers/trigo.h raydium/headers/vertex.h raydium/headers/window.h raydium/headers/reg_api.h raydium/headers/gui.h raydium/headers/live.h
	@echo "" > $(LOG_FILE)
	touch $@
	@echo "Headers found: $+"

compile:  raydium/compile/background.o raydium/compile/callback.o raydium/compile/camera.o raydium/compile/capture.o raydium/compile/clear.o raydium/compile/console.o raydium/compile/file.o raydium/compile/fog.o raydium/compile/init.o raydium/compile/internal.o raydium/compile/joy.o raydium/compile/key.o raydium/compile/land.o raydium/compile/light.o raydium/compile/log.o raydium/compile/main.o raydium/compile/mouse.o raydium/compile/network.o raydium/compile/normal.o raydium/compile/object.o raydium/compile/ode.o raydium/compile/osd.o raydium/compile/parser.o raydium/compile/particle2.o raydium/compile/php.o raydium/compile/profile.o raydium/compile/random.o raydium/compile/rayphp.o raydium/compile/register.o raydium/compile/render.o raydium/compile/signal.o raydium/compile/sky.o raydium/compile/sound.o raydium/compile/texture.o raydium/compile/timecall.o raydium/compile/trigo.o raydium/compile/vertex.o raydium/compile/window.o raydium/headers/reg_api.o raydium/headers/gui.o raydium/headers/live.o
	touch $@
	@echo "Files created: $+"


################################################################################
# Cleaning rules
################################################################################


clean:
	rm -ri $(COMPILE_DIR)	|| true
	rm -i libraydium.so*	|| true
	rm -i libraydium.a*	|| true

clean_f:
	rm -r $(COMPILE_DIR)	|| true
	rm libraydium.so*	|| true
	rm libraydium.a*	|| true


################################################################################
# All files
################################################################################

raydium/compile/background.o: raydium/background.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/callback.o: raydium/callback.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/camera.o: raydium/camera.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/capture.o: raydium/capture.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/clear.o: raydium/clear.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/console.o: raydium/console.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/file.o: raydium/file.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/fog.o: raydium/fog.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/init.o: raydium/init.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/internal.o: raydium/internal.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/joy.o: raydium/joy.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/key.o: raydium/key.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/land.o: raydium/land.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/light.o: raydium/light.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/log.o: raydium/log.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/main.o: raydium/main.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/mouse.o: raydium/mouse.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/network.o: raydium/network.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/normal.o: raydium/normal.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/object.o: raydium/object.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/ode.o: raydium/ode.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/osd.o: raydium/osd.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/parser.o: raydium/parser.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/particle2.o: raydium/particle2.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/php.o: raydium/php.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/profile.o: raydium/profile.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/random.o: raydium/random.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/rayphp.o: raydium/rayphp.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/register.o: raydium/register.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/render.o: raydium/render.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/signal.o: raydium/signal.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/sky.o: raydium/sky.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/sound.o: raydium/sound.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/texture.o: raydium/texture.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/timecall.o: raydium/timecall.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/trigo.o: raydium/trigo.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/vertex.o: raydium/vertex.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/window.o: raydium/window.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/reg_api.o: raydium/reg_api.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/gui.o: raydium/gui.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)

raydium/compile/live.o: raydium/live.c headers
	@mkdir -p $(COMPILE_DIR)
	@echo "Creating: $@"
	$(CC) $(COMPILE_OPTIONS) $(CFLAGS) -o $@ -c $< $(INCLUDE_PATH) 2>> $(LOG_FILE)
