#include <iostream>
#include <string>

using namespace std;

#include "../include/constants.hpp"
#include "../include/bb_dfs.hpp"
#include "../include/loader.hpp"
#include "../include/tester.hpp"

void runTester(string filePath, int a)
{
  // Load data from the particular file
  DATA d = runLoader(filePath, a);

  runBbDfs(d);
} // RUN TESTER
