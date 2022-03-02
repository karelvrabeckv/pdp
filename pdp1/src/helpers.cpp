#include <iostream>
#include <vector>

using namespace std;

#include "../include/constants.h"
#include "../include/helpers.h"

void outputVar(int var)
{
  cout << var << endl;
} // OUTPUT VARIABLE

void outputVec(INT_VEC vec)
{
  for (int el: vec)
  {
    cout << el << " ";
  }
  cout << endl;
} // OUTPUT VECTOR

void outputMat(INT_MAT mat)
{
  for (INT_VEC row: mat)
  {
    outputVec(row);
  }
} // OUTPUT MATRIX

void outputSol(SOLUTION sol, int numOfVertices)
{
  cout << "Found " << sol.quantity << " solution/s:" << endl;

  // Loop through the solutions
  for (int i = 0; i < (int)sol.indicators.size(); i++)
  {
    // Output the vertices of the set X
    cout << "[X]: { ";
    for (int j = 0; j < numOfVertices; j++)
    {
      if (!sol.indicators[i][j])
      {
        cout << j << " ";
      }
    }
    cout << "}, ";

    // Output the vertices of the set Y
    cout << "[Y]: { ";
    for (int j = 0; j < numOfVertices; j++)
    {
      if (sol.indicators[i][j])
      {
        cout << j << " ";
      }
    }
    cout << "}, ";

    cout << "[NUMBER OF EDGES]: " << sol.numOfEdges[i] << ", ";
    cout << "[WEIGHT]: " << sol.weight << endl;
  }

  cout << "Number of recursive calls: " << sol.numOfCalls << endl;
} // OUTPUT SOLUTION
