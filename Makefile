# this target will compile the file
all: lab4
	gcc lab4.c -o lab4
lab: lab.o
	gcc lab.o -o lab4
lab4.o: lab4.c
	gcc -c lab4.c
clean:
	rm -rf *o lab