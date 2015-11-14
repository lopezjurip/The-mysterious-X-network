default: all

build:
	gcc -lm -O2 -std=c89 -pipe -ansi main.c -o ./main

run:
	./main < sample.txt

clean:
	rm ./main

all:
	make build
	make run
	make clean
