#include <iostream>
#include "HPC.h"

bool print_values = false;
bool evaluation_test = false;

void test_matrix_multiplication(const size_t& size, HPC& hpc)
{
	Matrix matrix(size + 1, size);

	double start, finish, duration;

	start = MPI_Wtime();

	Vector result = hpc.solve_linear_equation_system(matrix);

	finish = MPI_Wtime();
	duration = finish - start;

	if (print_values)
	{
		size_t outputWide = 10;
		matrix.set_output_wide(outputWide);

		std::cout << "Result:" << std::endl << result;
	}

	std::cout << "Matrix size: " << size << std::endl;
	std::cout << "Time of execution: " << std::fixed << std::setprecision(12) << duration << std::endl << std::endl;

	if (result == matrix.solve_linear_equation_system())
	{
		std::cout << "The results of serial and parallel algorithms are identical!" << std::endl;
	}
	else
	{
		std::cout << "The results of serial and parallel algorithms are NOT identical!" << std::endl;
	}
}

int main(int argc, char* argv[])
{
	srand(clock());

	for (size_t i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-p") == 0)
		{
			print_values = true;
		}
		else if (strcmp(argv[i], "-t") == 0)
		{
			evaluation_test = true;
		}
	}

	HPC hpc(argc, argv);

	size_t* evaluation_sizes = new size_t[9]{ 10, 100, 500, 1000, 1500, 2000, 2500, 3000 };

	if (hpc.get_process_rank() == 0)
	{
		if (evaluation_test)
		{
			for (size_t i = 0; i < 8; i++)
			{
				if (hpc.get_process_num() < evaluation_sizes[i])
				{
					Matrix matrix_left(evaluation_sizes[i]);
					Matrix matrix_right(evaluation_sizes[i]);
					matrix_left.random_data_initialization();
					matrix_right.random_data_initialization();

					std::cout << std::endl << "Matrix size " << evaluation_sizes[i] << "x"
						<< evaluation_sizes[i] << ":" << std::endl;

					test_matrix_multiplication(evaluation_sizes[i], hpc);
				}
			}

			delete[] evaluation_sizes;
			return 0;
		}

		size_t size;
		std::cout << "Enter size of matrix:";
		std::cin >> size;
		test_matrix_multiplication(size, hpc);
	}
	else
	{
		if (evaluation_test)
		{
			for (size_t i = 0; i < 8; i++)
			{
				hpc.solve_linear_equation_system();
			}
		}
		else
		{
			hpc.solve_linear_equation_system();
		}
	}

	delete[] evaluation_sizes;
	return 0;
}