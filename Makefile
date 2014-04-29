GPP= g++
COMPILE= --std=gnu++11 -c -Iinclude/ -I/usr/local/include
LIB= -Llib/ -L/usr/local/lib
LIBS= -lsndfile -lfftw3 `pkg-config --cflags --libs allegro-5 allegro_main-5 allegro_primitives-5` -lm
STD= --std=gnu++11

default: silo.out

debug: COMPILE+= -g
debug: silo.out

silo.out: silo.o
	$(GPP) $(LIB) $(LIBS) silo.o -o silo.out

silo.o: Main.cpp
	$(GPP) $(COMPILE) Main.cpp -o silo.o

clean:
	rm *.o

