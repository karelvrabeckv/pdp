# Variables
FLAGS = -Wall -pedantic -O3 -std=c++11 -fopenmp
DEPENDENCIES = main.cpp src/core.cpp src/helpers.cpp src/loader.cpp
PROGRAM_NAME = run

# Build the program
$(PROGRAM_NAME): delete_program make_program

delete_program:
	rm -f $(PROGRAM_NAME)

make_program:
	OMPI_CXX=g++ mpic++ $(FLAGS) $(DEPENDENCIES) -o $(PROGRAM_NAME)
