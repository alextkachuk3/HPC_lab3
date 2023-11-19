#include "Matrix.h"

Matrix::Matrix(const size_t& size)
{
	outputWide = defaultOutputWide;

	this->width = size;
	this->height = size;

	values = new double[size * size] {0};
}

Matrix::Matrix(const size_t& width, const size_t& height)
{
	outputWide = defaultOutputWide;

	this->width = width;
	this->height = height;

	values = new double[width * height] {0};
}

Matrix::Matrix(const double* values, const size_t& width, const size_t& height)
{
	this->values = new double[width * height];
	this->width = width;
	this->height = height;
	memcpy_s(this->values, this->width * this->height * sizeof(double), values, width * height * sizeof(double));
}

Matrix::~Matrix()
{
	delete[] values;
}

Vector Matrix::solve_linear_equation_system() const
{
	Vector result(height);

	Matrix copy_matrix(*this);

	copy_matrix.gaussian_elimination();
	copy_matrix.back_substitution();

	for (size_t i = 0; i < height; i++)
	{
		result[i] = copy_matrix.values[(i + 1) * copy_matrix.width - 1] / copy_matrix.values[i * (copy_matrix.width + 1)];
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

double*& Matrix::get_values()
{
	return values;
}

void Matrix::set_output_wide(const size_t& outputWide)
{
	this->outputWide = outputWide;
}

Matrix::Matrix(const Matrix& matrix)
{
	values = new double[matrix.width * matrix.height];
	width = matrix.width;
	height = matrix.height;
	memcpy_s(values, width * height * sizeof(double), matrix.values, matrix.width * matrix.height * sizeof(double));
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

void Matrix::gaussian_elimination()
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

void Matrix::back_substitution()
{
	double mult;
	for (size_t i = height - 1; i >= 1; i--)
	{
		for (size_t j = i; j >= 1; j--)
		{
			mult = values[(j - 1) * width + i] / values[i * (width + 1)];
			for (size_t k = width - 1; k >= j; k--)
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
