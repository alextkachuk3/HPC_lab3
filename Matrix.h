#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Vector.h"

class Matrix
{
public:
	Matrix() = default;
	Matrix(const size_t& size);
	Matrix(const size_t& width, const size_t& height);
	Matrix(const double* values, const size_t& width, const size_t& height);
	~Matrix();

	Vector solve_linear_equation_system() const;
	void gaussian_elimination();
	void back_substitution();

	void dummy_data_initialization();
	void random_data_initialization();

	std::string to_string() const;
	size_t get_width() const;
	size_t get_height() const;
	double*& get_values();

	void set_output_wide(const size_t& outputWide);

	Matrix(const Matrix& matrix);

	bool operator==(const Matrix& other);
	double& operator[](const size_t& index) const;
	friend Matrix operator*(const Matrix& left, const Matrix& right);
	Matrix& operator+=(const Matrix& other);

	friend std::ostream& operator<< (std::ostream& out, const Matrix& matrix);

private:
	size_t height;
	size_t width;
	double* values;

	size_t outputWide;
	const static size_t defaultOutputWide = 8;
};
