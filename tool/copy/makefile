cc = gcc
cflag = -c
OBJ = copy.o
BIN = objcopy textcopy

.DEFAULT_GOAL := all

all: $(BIN)

%.o: %.c
	$(cc) $(cflag) -o $@ $^

objcopy: $(OBJ) objcopy.o
	$(cc) -o $@ $^

textcopy: $(OBJ) textcopy.o
	$(cc) -o $@ $^

clean:
	rm *.o