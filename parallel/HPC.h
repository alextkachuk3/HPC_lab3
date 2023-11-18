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

	int height;
	int width;

	int distibution_size;
	int* distribution_rows;
	int* distribution_count;
	int* distribution_index;

	double* process_rows;
	double* process_result;

	void calculate_distribution();
	void distribute_matrix(double* matrix = nullptr);

	void log(std::string message);
};

