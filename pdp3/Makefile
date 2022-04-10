# Variables
COMPILER = g++
FLAGS = -Wall -pedantic -O3 -std=c++11
DEPENDENCIES = main.cpp src/bb_dfs.cpp src/loader.cpp src/tester.cpp
PROGRAM_NAME = run

# Build the program
$(PROGRAM_NAME): delete_program make_program

delete_program:
	rm -f $(PROGRAM_NAME)

make_program:
	$(COMPILER) $(FLAGS) -fopenmp $(DEPENDENCIES) -o $(PROGRAM_NAME)
