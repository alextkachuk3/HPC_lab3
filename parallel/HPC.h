#pragma once
#include "mpi.h"
#include "../Matrix.h"

class HPC
{
public:
	HPC(int argc, char* argv[]);
	~HPC();

	int get_process_rank() const;
	int get_process_num() const;

	Vector solve_linear_equation_system(Matrix& matrix);
	void solve_linear_equation_system();

private:
	int process_num;
	int process_rank;

	int height;
	int width;

	int distribution_rows_size;
	int distibution_size;
	int* distribution_rows;
	int* distribution_rows_index;
	int* distribution_count;
	int* distribution_index;

	double* process_rows;
	double* process_result;

	double* result;

	void calculate_distribution();
	void distribute_matrix(double* matrix = nullptr);

	void parallel_gaussian_elimination();
	void parallel_back_substitution();

	void result_collection();

	void log(std::string message);
};

