#include <iostream>

using namespace std;

#include "include/tester.hpp"

int main(int argc, char** argv)
{
   // Check the number of arguments
   if (argc != 3)
   {
      cout << "[ERROR] Two arguments expected." << endl;
      return 1;
   }

   string filePath(argv[1]);
   int a = atoi(argv[2]);

   runTester(filePath, a);

   return 0;
} // MAIN
