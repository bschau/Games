OBJS	=	build/main.o
DST	=	adventure
CC	=	gcc
CFLAGS	=	-c -O2 -Wall
DEPS	=	Makefile

all:	builddir $(DST)

builddir:
	test -d build || mkdir build

$(DST):	$(OBJS)
	$(CC) -o $(DST) build/*.o

clean:
	rm -f *.o *~
	rm -f $(DST)

distclean:	clean
	rm -fr build

build/main.o:	main.c $(DEPS)
	$(CC) $(CFLAGS) main.c -o build/main.o
