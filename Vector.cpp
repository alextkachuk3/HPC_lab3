#include "Vector.h"

Vector::Vector(const size_t& size)
{
	this->size = size;
	values = new double[size] {};
}

Vector::Vector(const double* values, const size_t& size)
{
	this->values = new double[size];
	this->size = size;
	memcpy_s(this->values, this->size * sizeof(double), values, size * sizeof(double));
}

Vector::~Vector()
{
	delete[] values;
}

std::string Vector::to_string() const
{
	std::stringstream string;
	for (size_t i = 0; i < size; i++)
	{
		string << std::fixed << std::setprecision(4) << values[i] << std::endl;
	}
	return string.str();
}

Vector::Vector(const Vector& vector)
{
	values = new double[vector.size];
	size = vector.size;
	memcpy_s(values, size * sizeof(double), vector.values, vector.size * sizeof(double));
}

void Vector::dummy_data_initialization()
{
	for (size_t i = 0; i < size; i++)
	{
		values[i] = 1;
	}
}

void Vector::random_data_initialization()
{
	srand(clock());
	for (size_t i = 0; i < size; i++)
	{
		values[i] = rand() / 1000.0;
	}
}

size_t Vector::get_size() const
{
	return size;
}

double* Vector::get_values() const
{
	return values;
}

std::ostream& operator<<(std::ostream& out, const Vector& vector)
{
	out << vector.to_string();

	return out;
}

bool Vector::operator==(const Vector& other)
{
	const static double accuracy = 1.e-6;

	if (this->size != other.size)
	{
		return false;
	}

	for (size_t i = 0; i < this->size; i++)
	{
		if (fabs(other.values[i] - values[i] >= accuracy))
		{
			return false;
		}
	}

	return true;
}

double& Vector::operator[](const size_t& index) const
{
	return values[index];
}
