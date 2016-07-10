#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <fstream>
#include <complex>

class Matrix
{
public:
	Matrix (const long num_rows, const long num_columns);
	Matrix & cross (Matrix & arg_vect);
	T dot (Matrix<double> & arg_vect);
	T& operator()(long row, long col) const;
	long getLength();
	void normalize();
	void calculate_Centroids_and_Normals (Matrix & centroids, Matrix & normals, long num_triangles, Matrix & nodes, Matrix & triangles);
	Matrix * build_nodes (long num_columns, long start_of_node_field, long start_of_triangle_field);
	Matrix * build_triangles (long num_columns, long start_of_triangle_field, long end_of_triangle_field);



//private:
	T ** matrix; //Matrix class was templatized to support complex numbers
	long number_of_rows;
};

