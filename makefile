CC = gcc
MAIN_OUT = a.out
INPUT = input.txt
OUTPUT = out.txt

.PHONY: all
all: main

main: test_data
	gcc -o $(MAIN_OUT) main.c

test_data:
	fallocate -l 1gb $(INPUT)

clean:
	rm -f $(MAIN_OUT) $(INPUT) $(OUTPUT)
