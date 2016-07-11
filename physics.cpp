#include "physics.h"

Matrix<int> * calculateIlluminatedTriangles (Matrix<double> & torch, Matrix<double> & normals)
{
	/* Takes on torch vector and normal matrix. Torch vector is assumed to be a 1 x 3 vector, incidicates direction of incident E-field
 * 	normals represents normal vector of each triangle in the mesh. Assumed to be n x 3. 
 * 	Function will take cross product of every row of normals with torch vector, return a n x 1 matrix consisting of 1's and zeroes.
 * 	The returned matrix data can be interpreted as : 1 - Corresponding triangle is illuminated by torch. 0 - Corresponding triangle is not illuminated by torch.
 * 	Function will allocate memory for return matrix, and must be freed by the caller. */
	/* Notes: I decided to switch to this implementation, where the row is extracted seperately from the normals matrix and passed into a dot product method that
	returns the result of a single operation, because it would be the most efficient way while maintaining a generalized dot product method */

	Matrix<int> *illuminated = new Matrix<int> (normals.getLength(), 1); //allocate new matrix to return
	for (long i = 0; i < normals.getLength(); i++) { //iterate through all normal vectors
		Matrix<double> *current_normals_vector = new Matrix<double> (1, 3); //create a new vector to store the current row
		(*current_normals_vector) (0, 0) = normals (i, 0); //concatenate normals x y z entries into one vector
		(*current_normals_vector) (0, 1) = normals (i, 1);
		(*current_normals_vector) (0, 2) = normals (i, 2);
		(*illuminated) (i, 0) = (*current_normals_vector).dot(torch); // (torch, normals (i, current_normals_vector)); //take dot product of current normals row with the torch and store in return value array
		if ((*illuminated) (i, 0) < -.00000000000001) //arbitrary threshold around 0 to determine 1/0 cutoff
			(*illuminated) (i, 0) = 1; //illuminated
		else
			(*illuminated)(i, 0) = 0; //not illuminated
		delete current_normals_vector; //Must delete the allocated vector, avoid a memory leak
	}
	return  illuminated;
}

Matrix<complex<double> > * generateEFieldIncident (Matrix<int> * illuminated, Matrix<double> & polarizing_vector, Matrix<double> & torch, Matrix<double> * centroids)
{
	//Q: The result of this function is one dimensional. Is this correct? 
	/* This function will generate an incident E field according to Electromagnetic wave equations
	illuminated points to matrix incidicating binary indicator that E field is incidient on triangle or not
	polarizing vector is a property of the E field, arbitrary
	torch: direction of propgation of the incident field
	centroids : matrix containing x y z coordinates of the centroids of the triangles in the mesh
	INSERT LAMBDA CONSTRAINTS HERE 
	*/

	int lambda = 138; //need to verify that this is, in fact, lambda. what
	Matrix<complex<double> > * IncidentElectricField = new Matrix<complex<double> > (centroids->getLength(), 3); //incident E field
	for (long j  = 0; j < centroids->getLength(); j++) { 
		std::complex<double> complex_number (0,0); //complex number placeholder
		std::complex<double> i(0, 1); //the complex variable i
		Matrix<double> * cur_centroid = new Matrix <double>(1, 3); //dot product precalculation
		(*cur_centroid)(0, 0)  = (*centroids) (j, 0); //set equal to the current centroid in the matrix
		(*cur_centroid)(0, 1)  = (*centroids) (j, 1);
		(*cur_centroid)(0, 2) = (*centroids) (j, 2);
		complex_number = exp (i *(lambda * PI) *  torch.dot(*cur_centroid)); //wave Eqn
	  	(*IncidentElectricField) (j, 0) = (*illuminated) (j, 0) * polarizing_vector(0, 0) * complex_number; //implemented vector multiplication manually
		(*IncidentElectricField) (j, 1) = (*illuminated) (j, 0) * polarizing_vector(0, 1) * complex_number; //no need to transpose polarizing vector
		(*IncidentElectricField) (j, 2) = (*illuminated) (j, 0) * polarizing_vector(0, 2) * complex_number;
	}
	return IncidentElectricField;
}
