CC=g++
FLAGS= -std=c++11 -g
EXECUTABLE=test-server

SRC=main.cpp test-server.cpp

all: $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(EXECUTABLE)

clean:
	rm -f $(OBJ) $(EXECUTABLE)
