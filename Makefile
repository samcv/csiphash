
all:
	gcc -O3 test.c -g -Wall -Wextra -ggdb -o siphashtest && ./siphashtest
