all: test

test: snake.cpp
	g++ -I /usr/include/SDL2 -lSDL2 snake.cpp -o test

run: test
	./test

clean:
	rm -f test
