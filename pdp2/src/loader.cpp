#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#include "../include/constants.h"
#include "../include/loader.h"

DATA runLoader(string filePath, int a)
{
  // Read from the text file
  ifstream file(DATA_FOLDER + filePath);

  // Load the number of vertices
  int numOfVertices;
  file >> numOfVertices;

  // Initialize the matrix of edges
  int** matOfEdges = new int*[numOfVertices];
  for (int i = 0; i < numOfVertices; i++)
    matOfEdges[i] = new int[numOfVertices];

  // Load the matrix of edges
  for (int i = 0; i < numOfVertices; i++)
    for (int j = 0; j < numOfVertices; j++)
      file >> matOfEdges[i][j];

  // Close the text file
  file.close();

  return { numOfVertices, a, matOfEdges };
} // RUN LOADER
