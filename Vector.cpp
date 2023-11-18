#include "Vector.h"

Vector::Vector(const size_t& size)
{
	this->size = size;
	values = new double[size] {0};
	this->delete_counter = new int{ 1 };
}

Vector::~Vector()
{
	delete_counter--;
	if (delete_counter == 0)
	{
		delete[] values;
		delete delete_counter;
	}
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
	delete_counter = vector.delete_counter;
	delete_counter++;
	values = vector.values;
	size = vector.size;
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
	if (this->size != other.size)
	{
		return false;
	}

	for (size_t i = 0; i < this->size; i++)
	{
		if (this->values[i] != other.values[i])
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
