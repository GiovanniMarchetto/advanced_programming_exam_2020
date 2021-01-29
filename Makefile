# SRC = home.cpp           

CXX = c++
CXXFLAGS = -Wall -Wextra -g -std=c++17

EXE = $(SRC:.cpp=.x)

all: $(EXE)

clean:
	rm -f $(EXE) *~
