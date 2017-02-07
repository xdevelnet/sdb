all:
	true
prototype:
	cc --std=c99 main.c -o prototype -Wall -s -O0
clean:
	rm prototype
