/*
 physics.h
 All the methods that implement the physical optics approximation calculations
*/

#ifndef __PHYSICS_H_INCLUDED__
#define __PHYSICS_H_INCLUDED__

#include <cstdlib>
#include <cmath>
#include <complex>
#include "Matrix.hpp"
#define PI 3.141592653589793238463 //could use confirmation
using namespace std;


Matrix<int> * calculateIlluminatedTriangles (Matrix<double> & torch, Matrix<double> & normals);

Matrix<complex<double> > * generateEFieldIncident (Matrix<int> * illuminated, Matrix<double> & polarizing_vector, Matrix<double> & torch, Matrix<double> * centroids);

Matrix <complex<double> >* generateHFieldIncident (Matrix<double> & torch, complex<double> impedance, Matrix <complex<double> >* E_field_inc);

#endif
