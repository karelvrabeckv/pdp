#include <iostream>
#include <limits>
#include <queue>

using namespace std;

#include "../include/constants.hpp"
#include "../include/helpers.hpp"

// ========================================

void bfs(queue<STATE> & q, const unsigned limit, const DATA & data)
{
  while (q.size() < limit)
  {
    STATE state = q.front();

    // We have reached the bottom
    if (state.numOfMarked == data.numOfVertices)
      break;

    // Mark the next vertice as X
    STATE stateX = createNextState(state, 0, data);
    q.push(stateX);

    // Mark the next vertice as Y
    STATE stateY = createNextState(state, 1, data);
    q.push(stateY);

    delete[] state.indicators;
    q.pop();
  }
} // BFS

// ========================================

STATE createNextState(const STATE & prev, const short indicator, const DATA & data)
{
  short* indicators = new short[data.numOfVertices];
  for (int i = 0; i < data.numOfVertices; i++)
    indicators[i] = prev.indicators[i];
  indicators[prev.numOfMarked] = indicator;

  STATE state = {prev.sizeOfX, prev.sizeOfY, prev.numOfMarked, indicators};
  
  if (indicator)
    state.sizeOfY++;
  else
    state.sizeOfX++;
  state.numOfMarked++;

  return state;
} // CREATE NEXT STATE

// ========================================

WEIGHT getWeight(const STATE & state, const DATA & data)
{
  int weight = 0, numOfEdges = 0;

  for (int i = 0; i < state.numOfMarked; i++)
  {
    // Only X (false) indicators remain
    if (state.indicators[i])
      continue;

    for (int j = 0; j < state.numOfMarked; j++)
    {
      // Only Y (true) indicators remain
      if (state.indicators[j])
      {
        // Get the weight of the edge
        weight += data.matOfEdges[i][j];

        // Increase the number of edges
        if (data.matOfEdges[i][j])
          numOfEdges++;
      }
    }
  }

  return {weight, numOfEdges};
} // GET WEIGHT

// ========================================

int getEstimatedWeight(const STATE & state, const DATA & data)
{
  int estimatedWeight = 0;

  // Loop through the remaining vertices
  for (int i = state.numOfMarked; i < data.numOfVertices; i++)
  {
    int weightX = 0, weightY = 0;

    // Loop through the evaluated vertices
    for (int j = 0; j < state.numOfMarked; j++)
    {
      if (state.indicators[j])
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

void initializeGlobalVariables(SOLUTION & best, const DATA & data)
{
  short* indicators = new short[data.numOfVertices];
  best = {numeric_limits<int>::max(), 0, 0, 0, indicators};
} // INITIALIZE GLOBAL VARIABLES

// ========================================

void deleteGlobalVariables(SOLUTION & best, DATA & data)
{
  delete[] best.indicators;

  for (int i = 0; i < data.numOfVertices; i++)
    delete[] data.matOfEdges[i];
  delete[] data.matOfEdges;
} // DELETE GLOBAL VARIABLES

// ========================================

vector<short> serializeState(const STATE & state, const DATA & data)
{
  vector<short> serializedState = {state.sizeOfX, state.sizeOfY, state.numOfMarked};
  for (int i = 0; i < data.numOfVertices; i++)
    serializedState.push_back(state.indicators[i]);

  return serializedState;
} // SERIALIZE STATE

// ========================================

STATE deserializeState(vector<short> & serializedState, const DATA & data)
{
  short* indicators = new short[data.numOfVertices];
  for (int i = 0; i < data.numOfVertices; i++)
    indicators[i] = serializedState[3 + i];

  return {serializedState[0], serializedState[1], serializedState[2], indicators};
} // DESERIALIZE STATE

// ========================================

vector<int> serializeSolution(const SOLUTION & best, const DATA & data)
{
  vector<int> serializedSolution = {best.weight, best.quantity, best.numOfCalls, best.numOfEdges};
  for (int i = 0; i < data.numOfVertices; i++)
    serializedSolution.push_back(best.indicators[i]);

  return serializedSolution;
} // SERIALIZE SOLUTION

// ========================================

SOLUTION deserializeSolution(vector<int> & serializedSolution, const DATA & data)
{
  short* indicators = new short[data.numOfVertices];
  for (int i = 0; i < data.numOfVertices; i++)
    indicators[i] = serializedSolution[4 + i];

  return {serializedSolution[0], serializedSolution[1], serializedSolution[2], serializedSolution[3], indicators};
} // DESERIALIZE SOLUTION

// ========================================

void outputResult(const SOLUTION & best, const double real_time, const double cpu_time, const DATA & data)
{
  cout << "Weight: " << best.weight << endl;
  cout << "Solutions: " << best.quantity << endl;
  cout << "Recursive calls: " << best.numOfCalls << endl;
  cout << "Edges: " << best.numOfEdges << endl;

  cout << "Indicators: { ";
  for (int i = 0; i < data.numOfVertices; i++)
    cout << (best.indicators[i] ? "1" : "0") << " ";
  cout << "}" << endl;

  cout << "Real time: " << real_time << " s" << endl;
  cout << "CPU time: " << cpu_time << " s" << endl;
} // OUTPUT RESULT
