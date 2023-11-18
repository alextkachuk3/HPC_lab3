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

Vector Matrix::serial_result_calculation()
{
	Vector result(height);

	serial_gaussian_elimination();
	serial_back_substitution();

	for (size_t i = 0; i < height; i++)
	{
		result[i] = values[(i + 1) * width - 1] / values[i * (width + 1)];
	}

	return result;
}

std::string Matrix::to_string() const
{
	std::stringstream string;
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			string << std::setw(outputWide) << std::fixed << std::setprecision(5) << values[i * width + j];
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

double& Matrix::operator[](const size_t& index) const
{
	return values[index];
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

void Matrix::serial_gaussian_elimination()
{
	double mult;
	for (size_t i = 0; i < height - 1; i++)
	{
		for (size_t j = i + 1; j < height; j++)
		{
			mult = values[j * width + i] / values[i * width + i];
			for (size_t k = i; k < width; k++)
			{
				values[j * width + k] -= (values[i * width + k] * mult);
			}
		}
	}
}

void Matrix::serial_back_substitution()
{
	double mult;
	for (size_t i = height - 1; i >= 1 ; i--)
	{
		for (size_t j = i; j >= 1; j--)
		{
			mult = values[(j - 1) * width + i] / values[i * (width + 1)];
			for (size_t k = width - 1; k >= j ; k--)
			{
				values[(j - 1) * width + k] -= (values[i * width + k] * mult);
			}
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
