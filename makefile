OBJECTS= x86.o
CC= gcc

x86: $(OBJECTS)
	$(CC) -o x86 $(OBJECTS)

x86.o: x86.c
	$(CC) -c x86.c

clean:
	rm *.o *~ x86