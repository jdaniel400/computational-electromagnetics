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



class Mesh
{
private:
		
	long start_of_node_field, start_of_triangle_field, end_of_triangle_field; //markers filled in as part of the parsing process
	double * data; //array to hold parsed file data
	void getNewSubstring (char *& result, char * charArray, int start_index_inclusive, int end_index_exclusive);
	Matrix <double> *nodes;
	Matrix <double> *triangles;
public:
	Mesh ();
	
	Matrix <double> *centroids;
	Matrix <double> *normals;

	void calculate_Centroids_and_Normals ();

	Matrix<double> * build_nodes ();

	Matrix<double> * build_triangles ();

	double * parseAndBuildData (const char *file_name);
	
	void importEMF (const char *file_name, Matrix<double> &direction_of_propagation, Matrix<double> &polarizing_vector, complex<double> &impedance, int &lambda);
};

#endif
