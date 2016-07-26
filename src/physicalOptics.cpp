#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <fstream>
#include <complex>
#include "Matrix.hpp"
#include "mesh.hpp"
#include "physics.hpp"
#include "Matrix.cpp"
using namespace std;
int main ()
{
	

	
	Mesh *mesh = new Mesh (); //create the mesh, constructor will parse file and build
	Matrix<double> torch = *new Matrix <double>(1, 3); //direction vector of incident E-field
	torch (0, 0) = 2; torch (0, 1) = 5; torch (0, 2) = 1; //completely arbitrary direction of the incident Electromagnetic field	
	
	Matrix<int> *illuminated = calculateIlluminatedTriangles (torch, *(mesh->normals)); //build the illuminated vector, binary representation of illuminated triangle/shadowed triangle for each triangle in the mesh
	
	Matrix<double> polarizing_vector = *new Matrix<double> (1, 3);
	polarizing_vector =  *new Matrix<double> (1,3);
	polarizing_vector (0,0) = 1; polarizing_vector (0,1) = 0; polarizing_vector (0, 2) = 0;
	
	//Matrix <complex<double> >* precalc_exponentials = new Matrix <complex<double> >(normals->getLength(), 3);

	Matrix <complex<double> >* EFinc = generateEFieldIncident (illuminated, polarizing_vector, torch, mesh->centroids);
	
	complex<double> impedance = 120 / PI;	
	Matrix <complex<double> >* HFinc = generateHFieldIncident (torch, impedance, EFinc);
	
	Matrix <complex<double> >* JPO = calcSurfaceCurrents_PHYSICAL_OPTICS (mesh->normals, HFinc);

	Matrix <double> * MagJPO = calcMagJPO (JPO);

 /*	for (int i = nodes->getLength() - 401; i < nodes->getLength(); i++)
		cout << "nodes" << i << ": " << (*nodes) (i, 0) << " " << (*nodes) (i, 1) << " " << (*nodes) (i, 2) << endl;
*/	
	//for (long i = triangles->getLength() - 31; i < triangles->getLength(); i++)
	//	cout << "normal " << i << ": " << (*normals)(i, 0) << " " << (*normals)(i, 1) << " " << (*normals)(i, 2) << endl;

	for (int i = EFinc->getLength() - 310; i < EFinc->getLength(); i++)
		cout << "E field inc " << i << ": " << (*JPO) (i, 0).real() << " + " << (*EFinc) (i, 0).imag() << "i " << (*EFinc) (i, 1).real() << " + " << (*EFinc) (i, 1).imag() << "i " << (*EFinc) (i, 2).real() << " + " << (*EFinc) (i, 2).imag() << "i " << endl;
	MagJPO->print();	
	/*for (int i = centroids->getLength() - 250; i < centroids->getLength(); i++)
		cout << "centroid " << i << ": " << (*illuminated)(i, 0) << endl ; //" " << (*centroids)(i, 1) << " " << (*centroids)(i, 2) << endl;
*/
	
	return 0;
}

