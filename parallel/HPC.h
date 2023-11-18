#pragma once
#include "mpi.h"
#include "../Matrix.h"

class HPC
{
public:
	HPC(int argc, char* argv[]);
	~HPC();

	int get_process_rank();
	int get_process_num();

	Vector solve_linear_equation_system(Matrix& matrix);
	void solve_linear_equation_system();

private:
	int process_num;
	int process_rank;
};

