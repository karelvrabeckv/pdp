void bbDfs(const STATE & state, const DATA & data);
void dataParallelism(const STATE & initialState, const DATA & data);
void master(MPI_Status & status, const PARAMS & params, const DATA & data);
void slave(MPI_Status & status, const PARAMS & params, const DATA & data);
void mpi(DATA & data);
