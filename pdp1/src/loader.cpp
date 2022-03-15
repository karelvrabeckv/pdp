#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "../include/constants.h"
#include "../include/loader.h"

FILE_DATA runLoader(string filePath)
{
  // Read from the text file
  ifstream file(DATA_FOLDER + filePath);

  // Load the number of vertices
  int numOfVertices;
  file >> numOfVertices;

  // Load the matrix of edges
  INT_MAT matOfEdges;

  // Read rows
  for (int i = 0; i < numOfVertices; i++)
  {
    int num;
    INT_VEC row;

    // Read columns
    for (int j = 0; j < numOfVertices; j++)
    {
      file >> num;
      row.push_back(num);
    }

    matOfEdges.push_back(row);
  }

  // Close the text file
  file.close();

  return { numOfVertices, matOfEdges };
} // RUN LOADER
