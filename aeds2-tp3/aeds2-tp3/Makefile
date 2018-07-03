all:
	gcc -Wall -Wextra -std=c11 -ggdb -pg -O0 main.c file.c arvoreb/arvoreb.c -o main `pkg-config --cflags --libs gtk+-2.0`
clean:
	rm main
