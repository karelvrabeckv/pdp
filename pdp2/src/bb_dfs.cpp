#include <iostream>
#include <chrono>
#include <time.h>
#include <string>

using namespace std;

#include "../include/constants.h"
#include "../include/bb_dfs.h"

DATA data;
SOLUTION best;

// ========================================

WEIGHT getWeight(bool* indicators, int numOfMarked)
{
  int weight = 0, numOfEdges = 0;

  for (int i = 0; i < numOfMarked; i++)
  {
    // Only X (false) indicators remain
    if (indicators[i])
      continue;

    for (int j = 0; j < numOfMarked; j++)
    {
      // Only Y (true) indicators remain
      if (indicators[j])
      {
        // Get the weight of the edge
        weight += data.matOfEdges[i][j];

        // Increase the number of edges
        if (data.matOfEdges[i][j])
          numOfEdges++;
      }
    }
  }

  return { weight, numOfEdges };
} // GET WEIGHT

// ========================================

int getEstimatedWeight(bool* indicators, int numOfMarked)
{
  int estimatedWeight = 0;

  // Loop through the remaining vertices
  for (int i = numOfMarked; i < data.numOfVertices; i++)
  {
    int weightX = 0, weightY = 0;

    // Loop through the evaluated vertices
    for (int j = 0; j < numOfMarked; j++)
    {
      if (indicators[j])
      {
        // The remaining vertice is marked as X
        // (false), so add the edge to Y (true)
        weightX += data.matOfEdges[i][j];
      }
      else
      {
        // The remaining vertice is marked as Y
        // (true), so add the edge to X (false)
        weightY += data.matOfEdges[i][j];
      }
    }

    estimatedWeight += (weightX > weightY ? weightY : weightX);
  }

  return estimatedWeight;
} // GET ESTIMATED WEIGHT

// ========================================

void bbDfs(int sizeOfX, int sizeOfY, int numOfMarked, bool* indicators)
{
  #pragma omp atomic
  best.numOfCalls++;

  // Check the condition of parameter a
  if (sizeOfX > data.a && sizeOfY > data.a)
    return;

  WEIGHT currWeight = getWeight(indicators, numOfMarked);
  int currEstimatedWeight = getEstimatedWeight(indicators, numOfMarked);

  // Check both conditions of weight
  if (best.weight)
    if (currWeight.weight > best.weight || currWeight.weight + currEstimatedWeight > best.weight)
      return;

  // Check the final condition
  if (numOfMarked == data.numOfVertices)
  {
    if (sizeOfX == data.a || sizeOfY == data.a)
    {
      #pragma omp critical
      {
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
        }

        best.numOfEdges[best.quantity - 1] = currWeight.numOfEdges;
        for (int i = 0; i < data.numOfVertices; i++)
          best.indicators[best.quantity - 1][i] = indicators[i];
      }
    }
    
    return;
  }

  // Mark the vertice as X
  bool* newIndicatorsX = new bool[numOfMarked + 1];
  copy(indicators, indicators + numOfMarked, newIndicatorsX);
  newIndicatorsX[numOfMarked] = false;

  #pragma omp task if (numOfMarked + 1 < data.numOfVertices - 2)
  {
    bbDfs(sizeOfX + 1, sizeOfY, numOfMarked + 1, newIndicatorsX);
    delete[] newIndicatorsX;
  }

  // Mark the vertice as Y
  bool* newIndicatorsY = new bool[numOfMarked + 1];
  copy(indicators, indicators + numOfMarked, newIndicatorsY);
  newIndicatorsY[numOfMarked] = true;

  #pragma omp task if (numOfMarked + 1 < data.numOfVertices - 2)
  {
    bbDfs(sizeOfX, sizeOfY + 1, numOfMarked + 1, newIndicatorsY);
    delete[] newIndicatorsY;
  }
} // BB DFS

// ========================================

void initializeGlobalVariables(DATA d)
{
  data = d;

  int* numOfEdges = new int[SIZE];
  bool** indicators = new bool*[SIZE];
  for (int i = 0; i < SIZE; i++)
    indicators[i] = new bool[d.numOfVertices];
  
  best = { 0, 0, 0, numOfEdges, indicators };
} // INITIALIZE GLOBAL VARIABLES

// ========================================

void deleteGlobalVariables()
{
  delete[] best.numOfEdges;

  for (int i = 0; i < SIZE; i++)
    delete[] best.indicators[i];
  delete[] best.indicators;

  for (int i = 0; i < data.numOfVertices; i++)
    delete[] data.matOfEdges[i];
  delete[] data.matOfEdges;
} // DELETE GLOBAL VARIABLES

// ========================================

void outputResult(double real_time, double cpu_time)
{
  cout << "Found " << best.quantity << " solution/s:" << endl;
  cout << endl;

  // Loop through the solutions
  for (int i = 0; i < best.quantity; i++)
  {
    cout << i + 1 << ". [INDICATORS]: { ";

    for (int j = 0; j < data.numOfVertices; j++)
      cout << (best.indicators[i][j] ? "1" : "0") << " ";

    cout << "}, ";
    cout << "[NUMBER OF EDGES]: " << best.numOfEdges[i] << ", ";
    cout << "[WEIGHT]: " << best.weight << endl;
  }

  cout << endl;
  cout << "Number of recursive calls: " << best.numOfCalls << endl;
  cout << "Real time: " << real_time << " s" << endl;
  cout << "CPU time: " << cpu_time << " s" << endl;
} // OUTPUT RESULT

// ========================================

void runBbDfs(DATA d)
{
  initializeGlobalVariables(d);
  
  int sizeOfX = 1, sizeOfY = 0;
  bool* indicators = new bool[1];
  indicators[0] = false; // to avoid symmetry
  
  auto real_time_start = chrono::steady_clock::now();
  auto cpu_time_start = clock();

  #pragma omp parallel firstprivate(sizeOfX, sizeOfY, indicators) num_threads(4)
  {
    #pragma omp single
    {
      bbDfs(sizeOfX, sizeOfY, sizeOfX + sizeOfY, indicators);
      delete[] indicators;
    }
  }

  auto cpu_time_end = clock();
  auto real_time_end = chrono::steady_clock::now();

  double real_time = chrono::duration_cast<chrono::milliseconds>(real_time_end - real_time_start).count() / 1000.0;
  double cpu_time = double(cpu_time_end - cpu_time_start) / CLOCKS_PER_SEC;

  outputResult(real_time, cpu_time);
  deleteGlobalVariables();
} // RUN BB DFS
