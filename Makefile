CC := gccx
CFLAGS := -g
TARGETS := encode decode
HEADERS := HuffmanTree.h functions.h
OBJECTS := encode.o decode.o functions.o main.o

all: $(TARGETS)

%.o: %.c HEADERS
	$(CC) $(CFLAGS) -c -o $@ $<

encode: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

decode: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS) *.o *~ *core*
