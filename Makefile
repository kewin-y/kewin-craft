CC = g++
CFLAGS = -Wall -Wextra
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -I./include

./bin/kc: ./src/*.cpp
	${CC} -o ./bin/kc ./src/*.cpp $(CFLAGS) $(LDFLAGS)

.PHONY: test clean

test: ./bin/kc
	./bin/kc

clean: ./bin/kc
	rm -f ./bin/kc

