#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#include "../include/constants.hpp"
#include "../include/loader.hpp"

DATA runLoader(const string filePath, const short a)
{
  // Read from the text file
  ifstream file(DATA_FOLDER + filePath);

  // Load the number of vertices
  short numOfVertices;
  file >> numOfVertices;

  // Initialize the matrix of edges
  short** matOfEdges = new short*[numOfVertices];
  for (int i = 0; i < numOfVertices; i++)
    matOfEdges[i] = new short[numOfVertices];

  // Load the matrix of edges
  for (int i = 0; i < numOfVertices; i++)
    for (int j = 0; j < numOfVertices; j++)
      file >> matOfEdges[i][j];

  // Close the text file
  file.close();

  return {numOfVertices, a, matOfEdges};
} // RUN LOADER
