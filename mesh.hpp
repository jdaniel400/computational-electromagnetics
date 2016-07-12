//mesh.h
/*All the functions necessary to build the mesh from the .unv file */
#ifndef __MESH_H_INCLUDED__
#define __MESH_H_INCLUDED__


#include <cstdlib>
#include <cmath>
#include <complex>
#include "Matrix.hpp"
#define PI 3.141592653589793238463 //could use confirmation

using namespace std;


void calculate_Centroids_and_Normals (Matrix<double> & centroids, Matrix<double> & normals, long num_triangles, Matrix<double> & nodes, Matrix<double> & triangles);

Matrix<double> * build_nodes (double * data, long start_of_node_field, long start_of_triangle_field);

Matrix<double> * build_triangles (double * data, long start_of_triangle_field, long end_of_triangle_field);

void getNewSubstring (char *& result, char * charArray, int start_index_inclusive, int end_index_exclusive);

double * parseAndBuildData (const char *file_name, long & start_of_node_field, long & start_of_triangle_field, long & end_of_triangle_field);
#endif
