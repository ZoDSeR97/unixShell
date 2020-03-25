CC=gcc -g -Wall
CFLAGS=-I.
DEPS = null
OBJ = shell.o  

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o shell
