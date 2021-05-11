TARGET=main

all: $(TARGET)

$(TARGET): $(TARGET).cpp benchmark.cpp resources.cpp benchmark1.cpp
	g++ -g -Wall -pthread -std=c++11 $^ -o $@

clean:
	rm -rf $(TARGET)

DEPS=benchmark.h resources.h benchmark1.h