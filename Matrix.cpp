#include "Matrix.h"

Matrix::Matrix(const size_t& size)
{
	outputWide = defaultOutputWide;
	submatrix_index = 0;

	this->width = size;
	this->height = size;

	values = new double[size * size] {0};
	this->delete_counter = new int{ 1 };
}

Matrix::Matrix(const size_t& width, const size_t& height)
{
	outputWide = defaultOutputWide;
	submatrix_index = 0;

	this->width = width;
	this->height = height;

	values = new double[width * height] {0};
	this->delete_counter = new int{ 1 };
}

Matrix::~Matrix()
{
	delete_counter--;
	if (delete_counter == 0)
	{
		delete[] values;
		delete delete_counter;
	}
}

Vector Matrix::serial_result_calculation(const Vector& vector)
{
	Vector result(vector.get_size());
	serial_pivot_pos = new size_t[height];
	serial_pivot_iter = new size_t[height];
	for (int i = 0; i < height; i++)
	{
		serial_pivot_iter[i] = -1;
	}

	serial_gaussian_elimination(*this, vector, height);
	serial_back_substitution(*this, vector, result, height);

	delete[] serial_pivot_pos;
	delete[] serial_pivot_iter;
	return result;
}

std::string Matrix::to_string() const
{
	std::stringstream string;
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			string << std::setw(outputWide) << std::fixed << std::setprecision(3) << values[i * width + j];
		}
		string << std::setw(0) << std::endl;
	}
	return string.str();
}

size_t Matrix::get_width() const
{
	return width;
}

size_t Matrix::get_height() const
{
	return height;
}

size_t Matrix::get_submatrix_index() const
{
	return submatrix_index;
}

double*& Matrix::get_values()
{
	return values;
}

void Matrix::set_submutrix_index(const size_t& submatrix_index)
{
	this->submatrix_index = submatrix_index;
}

void Matrix::set_output_wide(const size_t& outputWide)
{
	this->outputWide = outputWide;
}

Matrix::Matrix(const Matrix& matrix)
{
	delete_counter = matrix.delete_counter;
	delete_counter++;
	values = matrix.values;
	width = matrix.width;
	height = matrix.height;
	submatrix_index = matrix.submatrix_index;
	outputWide = matrix.outputWide;
}

bool Matrix::operator==(const Matrix& other)
{
	const static double accuracy = 1.e-6;

	if (this->width != other.width && this->height != other.height)
	{
		return false;
	}

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			if (fabs(other.values[i * width + j] - values[i * width + j] >= accuracy))
			{
				return false;
			}
		}
	}

	return true;
}

double* Matrix::operator[](const size_t& index) const
{
	return &values[index];
}

Matrix& Matrix::operator+=(const Matrix& other)
{
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			values[i * width + j] += other.values[i * width + j];
		}
	}
	return *this;
}

size_t Matrix::find_pivot_row(const Matrix& matrix, const size_t& size, const size_t& iter)
{
	size_t pivot_row = -1;
	int max_value = 0;

	for (size_t i = 0; i < size; i++) 
	{
		if ((serial_pivot_iter[i] == -1) &&	(fabs(*matrix[i * size + iter]) > max_value)) 
		{
			pivot_row = i;
			max_value = (int)fabs(*matrix[i * size + iter]);
		}
	}
	return pivot_row;
}

void Matrix::serial_column_elimination(const Matrix& matrix, const Vector& vector, const size_t& pivot, const size_t& iter, const size_t& size)
{
	double pivot_value, pivot_factor;
	pivot_value = *matrix[pivot * size + iter];
	for (size_t i = 0; i < size; i++) 
	{
		if (serial_pivot_iter[i] == -1) 
		{
			pivot_factor = *matrix[i * size + iter] / pivot_value;
			for (size_t j = iter; j < size; j++) {
				*matrix[i * size + j] -= pivot_factor * *matrix[pivot * size + j];
			}
			*vector[i] -= pivot_factor * *vector[pivot];
		}
	}
}

void Matrix::serial_gaussian_elimination(const Matrix& matrix, const Vector& vector, const size_t& size)
{
	size_t pivot_row;
	for (size_t iter = 0; iter < size; iter++)
	{
		pivot_row = find_pivot_row(matrix, size, iter);
		serial_pivot_pos[iter] = pivot_row;
		serial_pivot_iter[pivot_row] = iter;
		serial_column_elimination(matrix, vector, pivot_row, iter, size);
	}
}

void Matrix::serial_back_substitution(const Matrix& matrix, const Vector& vector, Vector& result, const size_t& size)
{
	size_t row_index, row;
	for (int i = (int)size - 1; i >= 0; i--) 
	{
		row_index = serial_pivot_pos[i];
		*result[i] = *vector[row_index] / *matrix[size * row_index + i];
		for (size_t j = 0; j < i; j++) {
			row = serial_pivot_pos[j];
			*vector[j] -= *matrix[row * size + i] * *result[i];
			*matrix[row * size + i] = 0;
		}
	}
}



void Matrix::dummy_data_initialization()
{
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			if (j <= i)
			{
				values[i * width + j] = 1.0;
			}
			else
			{
				values[i * width + j] = 0.0;
			}

		}
	}
}

void Matrix::random_data_initialization()
{
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			values[i * width + j] = rand() / 1000.0;
		}
	}
}

Matrix operator*(const Matrix& left, const Matrix& right)
{
	if (left.width != right.height)
	{
		throw std::invalid_argument("Left matrix width not equal right matrix height!");
	}
	Matrix result(right.width, left.height);

	for (int i = 0; i < left.height; i++)
	{
		for (int j = 0; j < right.width; j++)
		{
			for (int k = 0; k < right.height; k++)
			{
				result.values[i * right.width + j] += left.values[i * left.width + k] * right.values[k * right.width + j];
			}
		}
	}

	return result;
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
{
	out << matrix.to_string();

	return out;
}
