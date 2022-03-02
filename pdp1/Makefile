# Variables
CC = g++
CFLAGS = -Wall -pedantic -O3 -std=c++11
EXE = run

# Build the program
# $^ - all behind the colon
# $@ - all before the colon
./$(EXE): main.cpp src/bb_dfs.cpp src/helpers.cpp src/loader.cpp src/tester.cpp
	$(CC) $(CFLAGS) $^ -o $@
