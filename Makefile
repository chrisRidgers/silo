CPP= LD_RUN_PATH=/usr/local/lib g++ --std=gnu++11
INCLUDE= -Iinclude -I/usr/include -I/usr/local/include
COMPILE= -c
LIB= -Llib -L/usr/lib -L/usr/local/lib
LIBS= `pkg-config --cflags --libs allegro-5 allegro_main-5 allegro_primitives-5` -lfftw3 -lsndfile -lm

default: silo.out

silo.out: silo.o globals.o
	$(CPP) -o silo.out globals.o silo.o $(LIB) $(LIBS) 

silo.o: silo.cpp
	$(CPP) $(COMPILE) $(INCLUDE) silo.cpp -o silo.o

globals.o: globals.cpp
	$(CPP) $(COMPILE) $(INCLUDE) globals.cpp -o globals.o

clean:
	rm *.o *.out
	
