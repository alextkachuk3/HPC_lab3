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
	distribution_rows_size = 0;
	process_rows = 0;

	distribution_rows = new int[process_num];
	distribution_rows_index = new int[process_num];
	distribution_count = new int[process_num];
	distribution_index = new int[process_num];
}

HPC::~HPC()
{
	MPI_Finalize();
	delete[] distribution_rows;
	delete[] distribution_rows_index;
	delete[] distribution_count;
	delete[] distribution_index;
}

int HPC::get_process_rank() const
{
	return process_rank;
}

int HPC::get_process_num() const
{
	return process_num;
}

Vector HPC::solve_linear_equation_system(Matrix& matrix)
{
	height = (int)matrix.get_height();
	width = (int)matrix.get_width();

	MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);

	calculate_distribution();

	process_rows = new double[distribution_count[process_rank]];

	result = new double[height] {};

	MPI_Barrier(MPI_COMM_WORLD);

	distribute_matrix(matrix.get_values());

	parallel_gaussian_elimination();

	MPI_Barrier(MPI_COMM_WORLD);

	process_result = new double[distribution_rows[process_rank]];

	result_collection();

	delete[] process_result;
	delete[] process_rows;

	return Vector(result, (size_t)height);
}

void HPC::solve_linear_equation_system()
{
	MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);

	calculate_distribution();

	process_rows = new double[distribution_count[process_rank]] {};

	MPI_Barrier(MPI_COMM_WORLD);

	distribute_matrix();

	parallel_gaussian_elimination();

	MPI_Barrier(MPI_COMM_WORLD);

	process_result = new double[distribution_rows[process_rank]];

	result_collection();

	delete[] process_result;
	delete[] process_rows;
}

void HPC::log(std::string message)
{
	std::cout << ("Process rank:" + std::to_string(process_rank) + " " + message + "\n");
}

void HPC::distribute_matrix(double* matrix)
{
	MPI_Scatterv(matrix, distribution_count, distribution_index, MPI_DOUBLE, process_rows, distribution_count[process_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

void HPC::parallel_gaussian_elimination()
{
	int cur_process = 0;
	double mult = 0.0;
	double* row = new double[width] {};
	double* temp{};
	for (int i = 0; i < height; i++)
	{
		cur_process = i / distribution_rows_size;
		if (cur_process >= process_num)
		{
			cur_process = process_num - 1;
		}

		if (cur_process == process_rank)
		{
			MPI_Bcast(process_rows + ((i - distribution_rows_index[process_rank]) * width), width, MPI_DOUBLE, cur_process, MPI_COMM_WORLD);
			temp = row;
			row = process_rows + ((i - distribution_rows_index[process_rank]) * width);
		}
		else
		{
			MPI_Bcast(row, width, MPI_DOUBLE, cur_process, MPI_COMM_WORLD);
		}

		for (int j = 0; j < distribution_rows[process_rank]; j++)
		{
			if (distribution_rows_index[process_rank] + j != i)
			{
				mult = process_rows[j * width + i] / row[i];
				for (int k = i; k < width; k++)
				{
					process_rows[j * width + k] -= row[k] * mult;
				}
			}
		}

		if (cur_process == process_rank)
		{
			row = temp;
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

	delete[] row;
}

void HPC::calculate_distribution()
{
	distribution_rows_size = height / process_num;
	distibution_size = distribution_rows_size * width;

	std::fill_n(distribution_rows, process_num, distribution_rows_size);
	std::fill_n(distribution_count, process_num, distibution_size);

	distribution_rows[process_num - 1] += (height % process_num);
	distribution_count[process_num - 1] += (height % process_num) * width;

	distribution_index[0] = 0;
	distribution_rows_index[0] = 0;
	for (size_t i = 1; i < process_num; i++)
	{
		distribution_index[i] = distribution_index[i - 1] + distibution_size;
		distribution_rows_index[i] = distribution_rows_index[i - 1] + distribution_rows_size;
	}
}

void HPC::result_collection()
{
	for (int i = 0; i < distribution_rows[process_rank]; i++)
	{
		process_result[i] = process_rows[width * (i + 1) - 1] / process_rows[distribution_rows_index[process_rank] + (i * (width + 1))];
	}

	MPI_Gatherv(process_result, distribution_rows[process_rank], MPI_DOUBLE, result, distribution_rows, distribution_rows_index, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}
