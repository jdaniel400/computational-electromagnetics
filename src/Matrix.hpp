//Matrix.h
#pragma once

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <fstream>
#include <complex>
#define MAX_LINES 800000
#define MAX_CHARS_PER_LINE 93
#define PI 3.141592653589793238463 //could use confirmation
using namespace std;
template <class T>
class Matrix
{
public:
	Matrix (const long num_rows, const long num_columns);
	Matrix & cross (Matrix & arg_vect);
	T dot (Matrix<double> & arg_vect);
	T& operator()(long row, long col) const;
	T& operator*=(double c);
	long getLength();
	void normalize();
	void print ();
	T ** matrix; //Matrix class was templatized to support complex numbers
	long number_of_rows;
};

