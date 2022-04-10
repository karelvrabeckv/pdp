#include <iostream>
#include <mpi.h>

using namespace std;

#include "./include/constants.hpp"
#include "./include/core.hpp"
#include "./include/loader.hpp"

int main(int argc, char** argv)
{
   // Initialize the MPI environment
   MPI_Init(&argc, &argv);

   // Check the number of arguments
   if (argc != 3)
   {
      cout << "[ERROR] Two arguments expected." << endl;
      return 1;
   }

   string filePath(argv[1]);
   short a = (short) atoi(argv[2]);

   // Load data from the particular file
   DATA data = runLoader(filePath, a);

   // Process the data with MPI
   mpi(data);

   // Close the MPI environment
   MPI_Finalize();
} // MAIN
