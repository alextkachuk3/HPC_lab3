#include <iostream>
#include <iomanip>
#include <chrono>

#include "../Matrix.h"
#include "../Vector.h"

bool print_values = false;
bool evaluation_test = false;

void test_matrix_vector_multiplication(const size_t& size)
{
	Matrix matrix(size);
	Vector vector(size);
	matrix.random_data_initialization();
	vector.random_data_initialization();

	auto start = std::chrono::high_resolution_clock::now();

	Vector result = matrix.serial_result_calculation(vector);

	auto finish = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() * 1.0e-9;

	if (print_values)
	{
		size_t outputWide = 10;
		matrix.set_output_wide(outputWide);

		std::cout << "Matrix" << std::endl << matrix;
		std::cout << "Vector" << std::endl << vector;
		std::cout << "Result:" << std::endl << result;
	}

	std::cout << "Time of execution: " << std::fixed << std::setprecision(12) << duration << std::endl;
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

	if (evaluation_test)
	{
		size_t evaluation_sizes[] = { 10, 100, 500, 1000, 1500, 2000, 2500, 3000 };

		for (size_t i = 0; i < sizeof(evaluation_sizes) / sizeof(size_t); i++)
		{
			Matrix matrix_left(evaluation_sizes[i]);
			Matrix matrix_right(evaluation_sizes[i]);
			matrix_left.random_data_initialization();
			matrix_right.random_data_initialization();

			std::cout << std::endl << "Matrix size " << evaluation_sizes[i] << "x"
				<< evaluation_sizes[i] << ":" << std::endl;

			test_matrix_vector_multiplication(evaluation_sizes[i]);
		}
		return 0;
	}

	size_t size;

	std::cout << "Enter size of matrix:";

	std::cin >> size;

	test_matrix_vector_multiplication(size);

	return 0;

}
