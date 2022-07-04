OBJS	=	build/main.o build/player.o
DST	=	juggle
CC	=	gcc
CFLAGS	=	-c -O2 -Wall
DEPS	=	Makefile
LIBS	=	-lncurses

all:	builddir $(DST)

builddir:
	test -d build || mkdir build

$(DST):	$(OBJS)
	$(CC) -o $(DST) build/*.o $(LIBS)

clean:
	rm -f *.o *~
	rm -f $(DST)

distclean:	clean
	rm -fr build

build/main.o:	main.c $(DEPS)
	$(CC) $(CFLAGS) main.c -o build/main.o $(LIBS)

build/player.o:	player.c $(DEPS)
	$(CC) $(CFLAGS) player.c -o build/player.o $(LIBS)
