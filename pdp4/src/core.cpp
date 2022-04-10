#include <iostream>
#include <chrono>
#include <time.h>
#include <vector>
#include <queue>
#include <mpi.h>

using namespace std;

#include "../include/constants.hpp"
#include "../include/helpers.hpp"
#include "../include/core.hpp"

SOLUTION best;

// ========================================

void bbDfs(const STATE & state, const DATA & data)
{
  #pragma omp atomic
  best.numOfCalls++;

  // Check the condition of parameter a
  if (state.sizeOfX > data.a && state.sizeOfY > data.a)
    return;

  WEIGHT currWeight = getWeight(state, data);
  int currEstimatedWeight = getEstimatedWeight(state, data);

  // Check both conditions of weight
  if (currWeight.weight > best.weight || currWeight.weight + currEstimatedWeight > best.weight)
    return;

  // Check the final condition
  if (state.numOfMarked == data.numOfVertices)
  {
    if (state.sizeOfX == data.a || state.sizeOfY == data.a)
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
          best.numOfEdges = currWeight.numOfEdges;
          for (int i = 0; i < data.numOfVertices; i++)
            best.indicators[i] = state.indicators[i];
        }
      }
    }
    
    return;
  }

  // Mark the vertice as X
  STATE stateX = createNextState(state, 0, data);
  bbDfs(stateX, data);
  delete[] stateX.indicators;

  // Mark the vertice as Y
  STATE stateY = createNextState(state, 1, data);
  bbDfs(stateY, data);
  delete[] stateY.indicators;
} // BB DFS

// ========================================

void dataParallelism(const STATE & initialState, const DATA & data)
{
  queue<STATE> q;
  q.push(initialState);
  bfs(q, 100, data);
  
  vector<STATE> states;
  while (!q.empty())
  {
    states.push_back(q.front());
    q.pop();
  }

  #pragma omp parallel for schedule(dynamic)
  for (auto state: states)
  {
    bbDfs(state, data);
    delete[] state.indicators;
  }
} // DATA PARALLELISM

// ========================================

void master(MPI_Status & status, const PARAMS & params, const DATA & data)
{
  short sizeOfX = 1, sizeOfY = 0, numOfMarked = 1;
  short* indicators = new short[data.numOfVertices];
  indicators[0] = 0; // to avoid symmetry

  queue<STATE> q;
  q.push({sizeOfX, sizeOfY, numOfMarked, indicators});
  bfs(q, 50, data);

  // Initial distribution of work to slaves
  int numOfWorkers = 0;
  for (int id = 1; id < params.numOfProcesses; id++)
  {
    if (q.empty())
      break;
    
    STATE state = q.front();
    vector<short> serializedState = serializeState(state, data);
    MPI_Send(&serializedState[0], params.stateSize, MPI_SHORT, id, TAG_WORK, MPI_COMM_WORLD);
    delete[] state.indicators;
    q.pop();

    numOfWorkers++;
  }
  
  // Further distribution of work to slaves
  while (numOfWorkers > 0)
  {
    vector<int> serializedSolution(params.solutionSize);
    MPI_Recv(&serializedSolution[0], params.solutionSize, MPI_INT, MPI_ANY_SOURCE, TAG_DONE, MPI_COMM_WORLD, &status);
    SOLUTION solution = deserializeSolution(serializedSolution, data);

    best.numOfCalls += solution.numOfCalls;
    if (solution.weight == best.weight)
      best.quantity++;
    else if (solution.weight < best.weight)
    {
      best.weight = solution.weight;
      best.quantity = solution.quantity;
      best.numOfEdges = solution.numOfEdges;
      for (int i = 0; i < data.numOfVertices; i++)
        best.indicators[i] = solution.indicators[i];
    }
    delete[] solution.indicators;

    if (!q.empty())
    {
      // There is another work to do
      STATE state = q.front();
      vector<short> serializedState = serializeState(state, data);
      MPI_Send(&serializedState[0], params.stateSize, MPI_SHORT, status.MPI_SOURCE, TAG_WORK, MPI_COMM_WORLD);
      delete[] state.indicators;
      q.pop();
    }
    else
    {
      // There is no more work to do
      numOfWorkers--;
    }
  }

  // Terminate all processes
  for (int id = 1; id < params.numOfProcesses; id++)
  {
    vector<short> state(params.stateSize);
    MPI_Send(&state[0], params.stateSize, MPI_SHORT, id, TAG_DONE, MPI_COMM_WORLD);
  }
} // MASTER

// ========================================

void slave(MPI_Status & status, const PARAMS & params, const DATA & data)
{
  while (true)
  {
    vector<short> serializedState(params.stateSize);
    MPI_Recv(&serializedState[0], params.stateSize, MPI_SHORT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    STATE state = deserializeState(serializedState, data);

    if (status.MPI_TAG == TAG_WORK)
    {
      best.weight = numeric_limits<int>::max();
      best.quantity = 0;
      best.numOfCalls = 0;
      best.numOfEdges = 0;
      
      dataParallelism(state, data);

      vector<int> solution = serializeSolution(best, data);
      MPI_Send(&solution[0], params.solutionSize, MPI_INT, 0, TAG_DONE, MPI_COMM_WORLD);
    }
    else
    {
      delete[] state.indicators;
      break;
    }
  }
} // SLAVE

// ========================================

void mpi(DATA & data)
{
  initializeGlobalVariables(best, data);

  MPI_Status status;
  PARAMS params = {0, 0, 3 + data.numOfVertices, 4 + data.numOfVertices};
  MPI_Comm_rank(MPI_COMM_WORLD, &params.processId);
  MPI_Comm_size(MPI_COMM_WORLD, &params.numOfProcesses);

  if (params.processId == 0)
  {
    auto real_time_start = chrono::steady_clock::now();
    auto cpu_time_start = clock();

    master(status, params, data);

    auto cpu_time_end = clock();
    auto real_time_end = chrono::steady_clock::now();

    double real_time = chrono::duration_cast<chrono::milliseconds>(real_time_end - real_time_start).count() / 1000.0;
    double cpu_time = double(cpu_time_end - cpu_time_start) / CLOCKS_PER_SEC;

    outputResult(best, real_time, cpu_time, data);
  }
  else
    slave(status, params, data);

  deleteGlobalVariables(best, data);
} // MPI
