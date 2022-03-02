#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "../include/constants.h"
#include "../include/bb_dfs.h"
#include "../include/loader.h"
#include "../include/tester.h"

void runTester(string filePath, int a)
{
  // Load data from the particular file
  FILE_DATA fd = runLoader(filePath);

  runDfs(fd, a);
} // RUN TESTER
