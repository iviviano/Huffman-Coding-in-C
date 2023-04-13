CC := gccx
CFLAGS := -g
TARGETS_1 := encode decode bits
HEADERS_1 := HuffmanTree.h functions.h
OBJECTS_1 := encode.o decode.o functions.o main.o
TARGETS_2 := encode2 decode2 bits
HEADERS_2 := HuffmanTree2.h functions2.h
OBJECTS_2 := encode2.o decode2.o functions2.o main2.o

all: $(TARGETS_1) $(TARGETS_2)

%.o: %.c $(HEADERS_1) $(HEADERS_2)
	$(CC) $(CFLAGS) -c -o $@ $<

encode: $(OBJECTS_1)
	$(CC) $(CFLAGS) -o $@ $^

decode: $(OBJECTS_1)
	$(CC) $(CFLAGS) -o $@ $^

encode2: $(OBJECTS_2)
	$(CC) $(CFLAGS) -o $@ $^

decode2: $(OBJECTS_2)
	$(CC) $(CFLAGS) -o $@ $^

bits:
	touch $@

clean:
	rm -f $(TARGETS_1) $(TARGETS_2) *.o *~ *core*
