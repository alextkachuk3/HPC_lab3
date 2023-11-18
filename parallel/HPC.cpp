#include "HPC.h"

HPC::HPC(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	setvbuf(stdout, 0, _IONBF, 0);

	size = 0;
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

	size = matrix.get_height();

	MPI_Bcast(&size, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

	solve_linear_equation_system();

	return result;
}

void HPC::solve_linear_equation_system()
{
	if (process_rank != 0)
	{
		MPI_Bcast(&size, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
	}

	log("size:" + std::to_string(size));

	size_t distibution_size = size / process_num;

	size_t* distribution = new size_t[process_num];

	std::fill_n(distribution, process_num, distibution_size);

	distribution[process_num - 1] += size % process_num;

	if (process_rank == 0)
	{
		log("distribution_size:" + std::to_string(distibution_size));

		for (size_t i = 0; i < process_num; i++)
		{
			log("i: " + std::to_string(i) + " " + "dist size: " + std::to_string(distribution[i]));
		}
	}


}

void HPC::log(std::string message)
{
	std::cout << ("Process rank:" + std::to_string(process_rank) + " " + message + "\n");
}
