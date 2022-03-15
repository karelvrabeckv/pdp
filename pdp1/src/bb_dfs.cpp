#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "../include/constants.h"
#include "../include/bb_dfs.h"
#include "../include/helpers.h"

// Calculate the weight of the cut
WEIGHT getWeight(INT_MAT & matOfEdges, BOOL_VEC & indicators)
{
  int weight = 0, numOfEdges = 0;

  for (int i = 0; i < (int)indicators.size(); i++)
  {
    // Only X (false) indicators remain
    if (indicators[i])
    {
      continue;
    }

    for (int j = 0; j < (int)indicators.size(); j++)
    {
      // Only Y (true) indicators remain
      if (indicators[j])
      {
        // Get the weight of the edge
        weight += matOfEdges[i][j];

        // Increase the number of edges
        if (matOfEdges[i][j])
        {
          numOfEdges++;
        }
      }
    }
  }

  return { weight, numOfEdges };
} // GET WEIGHT

// Calculate the estimated weight of the cut
int getEstimatedWeight(int numOfVertices, INT_MAT & matOfEdges, BOOL_VEC & indicators)
{
  int estimatedWeight = 0;

  // Loop through the remaining vertices
  for (int i = (int)indicators.size(); i < numOfVertices; i++)
  {
    int weightX = 0, weightY = 0;

    // Loop through the evaluated vertices
    for (int j = 0; j < (int)indicators.size(); j++)
    {
      if (indicators[j])
      {
        // Vertice is marked as X (false),
        // so find edges to Y (true)
        weightX += matOfEdges[i][j];
      }
      else
      {
        // Vertice is marked as Y (true),
        // so find edges to X (false)
        weightY += matOfEdges[i][j];
      }
    }

    estimatedWeight += (weightX > weightY ? weightY : weightX);
  }

  return estimatedWeight;
} // GET ESTIMATED WEIGHT

void bbDfs(int & numOfVertices, int & a, int sizeOfX, int sizeOfY, SOLUTION & best, INT_MAT & matOfEdges, BOOL_VEC indicators)
{
  best.numOfCalls++;

  // Check the condition of parameter a
  if (sizeOfX > a && sizeOfY > a)
  {
    return;
  }

  // Check the condition of weight
  if (best.weight && getWeight(matOfEdges, indicators).weight > best.weight)
  {
    return;
  }

  // Check the condition of estimated weight
  if (best.weight && getWeight(matOfEdges, indicators).weight + getEstimatedWeight(numOfVertices, matOfEdges, indicators) > best.weight)
  {
    return;
  }

  // Check the final condition
  if ((int)indicators.size() == numOfVertices)
  {
    if (sizeOfX == a || sizeOfY == a)
    {
      WEIGHT currWeight = getWeight(matOfEdges, indicators);

      if (currWeight.weight == best.weight)
      {
        // Equivalent solution
        best.quantity++;
      }
      else
      {
        // Better solution
        best.weight = currWeight.weight;
        best.quantity = 1;
        best.numOfEdges.clear();
        best.indicators.clear();
      }

      best.numOfEdges.push_back(currWeight.numOfEdges);
      best.indicators.push_back(indicators);
    }
    
    return;
  }

  // Mark the vertice as X
  BOOL_VEC newIndicatorsX = indicators;
  newIndicatorsX.push_back(false);
  bbDfs(numOfVertices, a, sizeOfX + 1, sizeOfY, best, matOfEdges, newIndicatorsX);

  // Mark the vertice as Y
  BOOL_VEC newIndicatorsY = indicators;
  newIndicatorsY.push_back(true);
  bbDfs(numOfVertices, a, sizeOfX, sizeOfY + 1, best, matOfEdges, newIndicatorsY);
} // BB DFS

void runDfs(FILE_DATA fd, int a)
{
  SOLUTION best = { 0, 0, 0, {}, {} };
  INT_MAT matOfEdges = fd.matOfEdges;
  BOOL_VEC indicators { false /* to avoid symmetry */ };
  int sizeOfX = 1, sizeOfY = 0;
  
  bbDfs(fd.numOfVertices, a, sizeOfX, sizeOfY, best, matOfEdges, indicators);
  outputSol(best, fd.numOfVertices);
} // RUN DFS
