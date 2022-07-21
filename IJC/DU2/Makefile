#LD_LIBRARY_PATH="." ./wordcount-dynamic file


CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g
OBJLIB = htab_hash_function.o htab_init.o htab_move.o htab_size.o htab_bucket_count.o htab_lookup_add.o htab_find.o htab_for_each.o htab_erase.o htab_clear.o htab_free.o

all: tail wordcount wordcount-dynamic

tail: tail.c
	$(CC) $(CFLAGS) tail.c -o tail

wordcount: wordcount.o io.o libhtab.a
	$(CC) $(CFLAGS) -o wordcount wordcount.o io.o -L . libhtab.a
	
wordcount-dynamic: wordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) -o wordcount-dynamic wordcount.o io.o libhtab.so
	
# Staticka kniznica
libhtab.a: $(OBJLIB)
	ar -rcs libhtab.a $(OBJLIB)
	
# Dynamicka kniznica
libhtab.so:	$(OBJLIB)
	$(CC) $(CFLAGS) -shared -fPIC -o libhtab.so $(OBJLIB)

# OBJECT FILES
io.o: io.c
	$(CC) $(CFLAGS) -c io.c
	
wordcount.o: wordcount.c
	$(CC) $(CFLAGS) -c wordcount.c

htab_hash_function.o: htab_hash_function.c
	$(CC) $(CFLAGS) -fPIC -c htab_hash_function.c

htab_init.o: htab_init.c
	$(CC) $(CFLAGS) -fPIC -c htab_init.c
	
htab_move.o: htab_move.c htab_lookup_add.o htab_find.o htab_clear.o
	$(CC) $(CFLAGS) -fPIC -c htab_move.c

htab_size.o: htab_size.c
	$(CC) $(CFLAGS) -fPIC -c htab_size.c

htab_bucket_count.o: htab_bucket_count.c
	$(CC) $(CFLAGS) -fPIC -c htab_bucket_count.c
	
htab_lookup_add.o: htab_lookup_add.c htab_hash_function.o htab_find.o
	$(CC) $(CFLAGS) -fPIC -c htab_lookup_add.c
	
htab_find.o: htab_find.c htab_hash_function.o
	$(CC) $(CFLAGS) -fPIC -c htab_find.c

htab_for_each.o: htab_for_each.c
	$(CC) $(CFLAGS) -fPIC -c htab_for_each.c

htab_erase.o: htab_erase.c htab_hash_function.o
	$(CC) $(CFLAGS) -fPIC -c htab_erase.c
	
htab_clear.o: htab_clear.c htab_bucket_count.o
	$(CC) $(CFLAGS) -fPIC -c htab_clear.c
	
htab_free.o: htab_free.c htab_clear.o
	$(CC) $(CFLAGS) -fPIC -c htab_free.c
	
clean:
	rm *.o tail wordcount wordcount-dynamic libhtab.a libhtab.so

zip:
	zip du2.zip *.c *.h Makefile
