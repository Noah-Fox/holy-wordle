COMPILE = g++ -Wall -Werror -std=c++20

all: holy-wordle.cc wordle.h
	$(COMPILE) holy-wordle.cc -o holy-wordle
