CPP= g++
INCLUDE= -Iinclude -I/usr/local/include
COMPILE= -c
LIB= -Llib -L/usr/local/lib
LIBS= `pkg-config --libs allegro-5 allegro_main-5 allegro_primitives-5` -lfftw3 -lsndfile -lm

default: silo.out

silo.out: silo.o globals.o
	$(CPP) $(LIB) $(LIBS) silo.o globals.o -o silo.out

silo.o: silo.cpp
	$(CPP) $(COMPILE) $(INCLUDE) silo.cpp -o silo.o

globals.o: globals.cpp
	$(CPP) $(COMPILE) $(INCLUDE) globals.cpp -o globals.o

clean:
	rm *.o *.out
	
