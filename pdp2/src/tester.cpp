#include <iostream>
#include <string>

using namespace std;

#include "../include/constants.h"
#include "../include/bb_dfs.h"
#include "../include/loader.h"
#include "../include/tester.h"

void runTester(string filePath, int a)
{
  // Load data from the particular file
  DATA d = runLoader(filePath, a);

  runBbDfs(d);
} // RUN TESTER
