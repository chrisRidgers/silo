CPP= LD_RUN_PATH=/usr/local/lib g++ --std=gnu++11
INCLUDE= -Iinclude -I/usr/include -I/usr/local/include
COMPILE= -c
LIB= -Llib -L/usr/lib -L/usr/local/lib
LIBS= `pkg-config --cflags --libs allegro-5 allegro_main-5 allegro_primitives-5` -lfftw3 -lsndfile -lm

default: silo.out

debug: COMPILE+= -g
debug: silo.out

silo.out: silo.o landscape.o
	$(CPP) -o silo.out landscape.o silo.o $(LIB) $(LIBS) 

silo.o: silo.cpp
	$(CPP) $(COMPILE) $(INCLUDE) silo.cpp -o silo.o

landscape.o: landscape.cpp
	$(CPP) $(COMPILE) $(INCLUDE) landscape.cpp -o landscape.o

clean:
	rm *.o *.out
	
