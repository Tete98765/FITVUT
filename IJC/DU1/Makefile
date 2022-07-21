COMPILER=gcc
FLAGS= -O2 -g -std=c11 -pedantic -Wall -Wextra
OBJSRC=primes.o eratosthenes.o error.o
OBJSRC1=error.o steg-decode.o  eratosthenes.o ppm.o

all: primes primes-i steg-decode

primes: $(OBJSRC) bitset.h
	$(COMPILER) $(FLAGS) $(OBJSRC) -o primes -lm

primes-i: $(OBJSRC) bitset.o
	$(COMPILER) $(FLAGS) -DUSE_INLINE $(OBJSRC) bitset.o -o primes-i -lm 

steg-decode: $(OBJSRC1) bitset.h
	$(COMPILER) $(FLAGS) $(OBJSRC1) -o steg-decode -lm	

primes.o: primes.c
	$(COMPILER) -c primes.c

bitset.o: bitset.c bitset.h
	$(COMPILER) -DUSE_INLINE -c bitset.c
	
eratosthenes.o: eratosthenes.c eratosthenes.h 
	$(COMPILER) -c eratosthenes.c 

error.o:  error.c error.h
	$(COMPILER) -c error.c

steg-decode.o: steg-decode.c
	$(COMPILER) -c steg-decode.c

ppm.o: ppm.c ppm.h
	$(COMPILER) -c ppm.c

run: primes primes-i
	./primes
	./primes-i
	./steg-decode du1-obrazek.ppm

clean: 
	rm primes
	rm primes-i
	rm steg-decode
	rm *.o

zip:
	zip du1.zip *.c *.h Makefile
