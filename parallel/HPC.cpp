#include "HPC.h"

HPC::HPC(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	setvbuf(stdout, 0, _IONBF, 0);

	height = 0;
	width = 0;
	distibution_size = 0;
	process_rows = 0;


	distribution_count = new int[process_num];
	distribution_index = new int[process_num];
}

HPC::~HPC()
{
	MPI_Finalize();
	delete[] distribution_count;
	delete[] distribution_index;
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

	height = (int)matrix.get_height();
	width = (int)matrix.get_width();

	MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);

	calculate_distribution();

	process_rows = new double[distribution_count[process_rank] * width];

	distribute_matrix(matrix.get_values());

	delete[] process_rows;

	return result;
}

void HPC::solve_linear_equation_system()
{
	MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);

	calculate_distribution();

	process_rows = new double[distribution_count[process_rank] * width];

	distribute_matrix();

	if (process_rank == 0)
	{
		log("distribution_size:" + std::to_string(distibution_size));

		for (size_t i = 0; i < process_num; i++)
		{
			log("i: " + std::to_string(i) + " " + "dist size: " + std::to_string(distribution_count[i]));
			log("i: " + std::to_string(i) + " " + "dist index: " + std::to_string(distribution_index[i]));
		}
	}

	std::string val;

	for (int i = 0; i < distribution_count[process_rank]; i++)
	{
		val += (std::to_string(process_rows[i]) + ", ");
	}
	log(val);
	
	delete[] process_rows;
}

void HPC::log(std::string message)
{
	std::cout << ("Process rank:" + std::to_string(process_rank) + " " + message + "\n");
}

void HPC::distribute_matrix(double* matrix)
{
	MPI_Scatterv(matrix, distribution_count, distribution_index, MPI_DOUBLE, process_rows, distribution_count[process_rank] * width, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

void HPC::calculate_distribution()
{
	distibution_size = (height / process_num) * width;

	std::fill_n(distribution_count, process_num, distibution_size);

	distribution_count[process_num - 1] += (height % process_num) * width;

	distribution_index[0] = 0;
	for (size_t i = 1; i < process_num; i++)
	{
		distribution_index[i] = distribution_index[i - 1] + distibution_size;
	}
}
