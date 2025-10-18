CXX = g++

CXXFLAGS = -I include -std=c++17 -O2

SRC = $(wildcard src/*.cpp) $(wildcard utils/*.cpp)

BIN = bin/traceroute

all: $(BIN)

$(BIN): $(SRC)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(BIN)
