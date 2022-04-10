const string DATA_FOLDER = "data/";

const short TAG_WORK = 0;
const short TAG_DONE = 1;

typedef struct {
  short numOfVertices, a;
  short** matOfEdges;
} DATA;

typedef struct {
  int processId, numOfProcesses, stateSize, solutionSize;
} PARAMS;

typedef struct {
  int weight, quantity, numOfCalls, numOfEdges;
  short* indicators;
} SOLUTION;

typedef struct {
  short sizeOfX, sizeOfY, numOfMarked;
  short* indicators;
} STATE;

typedef struct {
  int weight, numOfEdges;
} WEIGHT;
