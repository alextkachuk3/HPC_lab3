#include "HPC.h"

HPC::HPC(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	setvbuf(stdout, 0, _IONBF, 0);
}

HPC::~HPC()
{
	MPI_Finalize();
}

int HPC::get_process_rank()
{
	return process_rank;
}

int HPC::get_process_num()
{
	return process_num;
}

Vector HPC::solve_linear_equation_system(const Matrix& matrix)
{
	return Vector(matrix.get_height());
}

void HPC::solve_linear_equation_system()
{
}
