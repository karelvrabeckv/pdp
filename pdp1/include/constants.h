const string DATA_FOLDER = "data/";

typedef vector<bool> BOOL_VEC;
typedef vector<int> INT_VEC;
typedef vector<vector<int>> INT_MAT;

typedef struct {
  int numOfVertices;
  vector<vector<int>> matOfEdges;
} FILE_DATA;

typedef struct {
  int weight, quantity, numOfCalls;
  vector<int> numOfEdges;
  vector<vector<bool>> indicators;
} SOLUTION;

typedef struct {
  int weight, numOfEdges;
} WEIGHT;
