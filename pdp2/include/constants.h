const int SIZE = 20;
const string DATA_FOLDER = "data/";

typedef struct {
  int weight, quantity, numOfCalls;
  int* numOfEdges;
  bool** indicators;
} SOLUTION;

typedef struct {
  int numOfVertices, a;
  int** matOfEdges;
} DATA;

typedef struct {
  int weight, numOfEdges;
} WEIGHT;
