GPP= g++
INCLUDE= -Iinclude/
LIB= -Llib/ 
LIBS= -lsndfile -lfftw3 -lm

default: silo.out

silo.out: silo.o
	$(GPP) $(LIB) $(LIBS) silo.o -o silo.out

silo.o: Main.cpp
	$(GPP) $(INCLUDE) -c Main.cpp -o silo.o

clean:
	rm *.o

