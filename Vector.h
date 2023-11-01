#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>

class Vector
{
public:
	Vector(const size_t& size);
	~Vector();

	void dummy_data_initialization();
	void random_data_initialization();
	size_t get_size() const;
	double* get_values() const;

	std::string to_string() const;

	Vector(const Vector& vector);

	bool operator==(const Vector& other);
	double* operator[](const size_t& index) const;

	friend std::ostream& operator<< (std::ostream& out, const Vector& vector);

private:
	size_t size;
	double* values;

	int* delete_counter;
};
