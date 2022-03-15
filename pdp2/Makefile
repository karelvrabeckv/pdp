# Variables
COMPILER = g++
FLAGS = -Wall -pedantic -O3 -std=c++11
DEPENDENCIES = main.cpp src/bb_dfs.cpp src/loader.cpp src/tester.cpp
SEQUENTIAL_PROGRAM_NAME = seq
PARALLEL_PROGRAM_NAME = par

# Build both programs
all: $(SEQUENTIAL_PROGRAM_NAME) $(PARALLEL_PROGRAM_NAME)

# Build the sequential program
$(SEQUENTIAL_PROGRAM_NAME): delete_seq make_seq

delete_seq:
	rm -f $(SEQUENTIAL_PROGRAM_NAME)

make_seq:
	$(COMPILER) $(FLAGS) $(DEPENDENCIES) -o $(SEQUENTIAL_PROGRAM_NAME)

# Build the parallel program
$(PARALLEL_PROGRAM_NAME): delete_par make_par

delete_par:
	rm -f $(PARALLEL_PROGRAM_NAME)

make_par:
	$(COMPILER) $(FLAGS) -fopenmp $(DEPENDENCIES) -o $(PARALLEL_PROGRAM_NAME)
