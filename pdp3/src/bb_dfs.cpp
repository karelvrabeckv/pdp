#include <iostream>
#include <chrono>
#include <time.h>
#include <string>
#include <vector>
#include <queue>

using namespace std;

#include "../include/constants.hpp"
#include "../include/bb_dfs.hpp"

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

STATE createNextState(STATE prev, bool indicator)
{
  bool* indicators = new bool[prev.numOfMarked + 1];
  copy(prev.indicators, prev.indicators + prev.numOfMarked, indicators);
  indicators[prev.numOfMarked] = indicator;

  STATE s = { prev.sizeOfX, prev.sizeOfY, prev.numOfMarked + 1, indicators };
  
  if (indicator)
    s.sizeOfY++;
  else
    s.sizeOfX++;

  return s;
} // CREATE NEXT STATE

// ========================================

void bbDfs(STATE s)
{
  #pragma omp atomic
  best.numOfCalls++;

  // Check the condition of parameter a
  if (s.sizeOfX > data.a && s.sizeOfY > data.a)
    return;

  WEIGHT currWeight = getWeight(s.indicators, s.numOfMarked);
  int currEstimatedWeight = getEstimatedWeight(s.indicators, s.numOfMarked);

  // Check both conditions of weight
  if (best.weight)
    if (currWeight.weight > best.weight || currWeight.weight + currEstimatedWeight > best.weight)
      return;

  // Check the final condition
  if (s.numOfMarked == data.numOfVertices)
  {
    if (s.sizeOfX == data.a || s.sizeOfY == data.a)
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
          best.indicators[best.quantity - 1][i] = s.indicators[i];
      }
    }
    
    return;
  }

  // Mark the vertice as X
  STATE sX = createNextState(s, false);
  bbDfs(sX);
  delete[] sX.indicators;

  // Mark the vertice as Y
  STATE sY = createNextState(s, true);
  bbDfs(sY);
  delete[] sY.indicators;
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

void bfs(queue<STATE> & q)
{
  STATE s = q.front();

  // Mark the next vertice as X
  STATE sX = createNextState(s, false);
  q.push(sX);

  // Mark the next vertice as Y
  STATE sY = createNextState(s, true);
  q.push(sY);

  delete[] s.indicators;
  q.pop();
} // BFS

// ========================================

void runBbDfs(DATA d)
{
  initializeGlobalVariables(d);

  int sizeOfX = 1, sizeOfY = 0, numOfMarked = sizeOfX + sizeOfY;
  bool* indicators = new bool[1];
  indicators[0] = false; // to avoid symmetry

  queue<STATE> q;
  q.push({ sizeOfX, sizeOfY, numOfMarked, indicators });
  while (q.size() < 100)
    bfs(q);
  
  vector<STATE> states;
  while (!q.empty())
  {
    states.push_back(q.front());
    q.pop();
  }

  auto real_time_start = chrono::steady_clock::now();
  auto cpu_time_start = clock();

  #pragma omp parallel for schedule(dynamic)
  for (auto state: states)
  {
    bbDfs(state);
    delete[] state.indicators;
  }

  auto cpu_time_end = clock();
  auto real_time_end = chrono::steady_clock::now();

  double real_time = chrono::duration_cast<chrono::milliseconds>(real_time_end - real_time_start).count() / 1000.0;
  double cpu_time = double(cpu_time_end - cpu_time_start) / CLOCKS_PER_SEC;

  outputResult(real_time, cpu_time);
  deleteGlobalVariables();
} // RUN BB DFS
