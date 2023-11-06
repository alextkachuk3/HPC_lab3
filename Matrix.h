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
	~Matrix();

	Vector serial_result_calculation(const Vector& vector);

	void dummy_data_initialization();
	void random_data_initialization();

	std::string to_string() const;
	size_t get_width() const;
	size_t get_height() const;
	size_t get_submatrix_index() const;
	double*& get_values();

	void set_submutrix_index(const size_t& submatrix_index);
	void set_output_wide(const size_t& outputWide);

	Matrix(const Matrix& matrix);

	bool operator==(const Matrix& other);
	double* operator[](const size_t& index) const;
	friend Matrix operator*(const Matrix& left, const Matrix& right);
	Matrix& operator+=(const Matrix& other);

	friend std::ostream& operator<< (std::ostream& out, const Matrix& matrix);

private:

	size_t height;
	size_t width;
	size_t submatrix_index;
	double* values;

	size_t* serial_pivot_pos = nullptr;
	size_t* serial_pivot_iter = nullptr;

	int* delete_counter;

	size_t outputWide;
	const static size_t defaultOutputWide = 7;

	size_t find_pivot_row(const Matrix& matrix, const size_t& size, const size_t& iter);
	void serial_column_elimination(const Matrix& matrix, const Vector& vector, const size_t& pivot, const size_t& iter, const size_t& size);
	void serial_gaussian_elimination(const Matrix& matrix, const Vector& vector, const size_t& size);
	void serial_back_substitution(const Matrix& matrix, const Vector& vector, Vector& result, const size_t& size);
};
