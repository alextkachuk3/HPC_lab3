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

Vector HPC::solve_linear_equation_system(Matrix& matrix) 
{
	Vector result(matrix.get_height());

    size_t size = matrix.get_height();	

    MPI_Bcast(&size, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    return result;
}

void HPC::solve_linear_equation_system()
{
	size_t size = 0;
	MPI_Bcast(&size, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
	std::cout << ("Process rank:" + std::to_string(process_rank) + " " + "size:" + std::to_string(size) + '\n');
}