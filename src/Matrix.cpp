#include "Matrix.hpp"

template <class T>
Matrix<T>::Matrix (const long num_rows, const long num_columns)
{
	//Matrix class constructor
	matrix = new T*[num_columns];

	for (long i = 0; i < num_columns; i++)
		matrix[i] = new T [num_rows];

	for (long j = 0; j < num_columns; j++)
		for (long i = 0; i < num_rows; i++)
			matrix[j][i] = 0; //implemented this just because original MATLAB code called for zeros.
	number_of_rows = num_rows;
}


template <class T>
T& Matrix<T>::operator()(long row, long col) const
{
	//note that it is ROW (Y) and then COL (X). MATLAB CONVENTION FORMAT
	return matrix[col][row]; //will return reference to cell entry, but can be modified
}

template <class T>
T& Matrix<T>::operator*=(double c)
{
	for (int i = 0; i < getLength(); i++) {
		matrix[0][i] = matrix[0][i] * c; 
		matrix[1][i] = matrix[1][i] * c; 
		matrix[2][i] = matrix[2][i] * c; 
	}
}

template <class T>
long Matrix<T>::getLength()
{
	return number_of_rows;
}

template <class T>
Matrix<T> & Matrix<T>::cross (Matrix & arg_vect)
{https://github.com/jdaniel400/computational-electromagnetics.git
	/*Cross product implementation, iteratively for each row of the matrices
	assumes that template type supports * operator */
	Matrix<T> *result = new Matrix<T> (getLength(), 3);
	//This is the formula for cross product, implemented iteratively for each row of the matrices
	//Cross product for single vector argument (1 x 3)	
	if (arg_vect.getLength() == 1) {
		for (long i = 0; i < getLength(); i++) {
			(*result)(i,0) = matrix[1][i] * arg_vect(0,2) - matrix[2][i] * arg_vect(0,1); //note that matrix[][] convention is the opposite of (*matrix) (,) convetion
			(*result)(i,1) = matrix[2][i] * arg_vect(0,0) - matrix[0][i] * arg_vect(0,2); //it seemed  more natural to create the matrices in this way
			(*result)(i,2) = matrix[0][i] * arg_vect(0,1) - matrix[1][i] * arg_vect(0,0); //and the to flip in the (,) overload
		}
		return *result;
	}
	//Cross product for matrix of vectors argument (n x 3)
	for (long i = 0; i < getLength(); i++)
	{
		(*result)(i,0) = matrix[1][i] * arg_vect(i,2) - matrix[2][i] * arg_vect(i,1); //note that matrix[][] convention is the opposite of (*matrix) (,) convetion
		(*result)(i,1) = matrix[2][i] * arg_vect(i,0) - matrix[0][i] * arg_vect(i,2); //it seemed  more natural to create the matrices in this way
		(*result)(i,2) = matrix[0][i] * arg_vect(i,1) - matrix[1][i] * arg_vect(i,0); //and the to flip in the (,) overload
	}
	return *result;
}


template <class T>
T Matrix<T>::dot (Matrix<double> & arg_vect)
{	
	/*Function takes on a Matrix assumed to be a 1 x 3 vector. arg_vect must be 1 x 3, no other size will be acceptable.
 * 	Update: This function assumes that this Matrix is a 1 x 3 vector.
 https://github.com/jdaniel400/computational-electromagnetics.git* 	Returns result as a double floating point*/
	return matrix[0][0] * arg_vect (0, 0) + matrix[1][0] * arg_vect (0, 1) + matrix[2][0] * arg_vect (0, 2);		
}
template <class T>
void Matrix<T>::normalize ()
{
	/* Assumes that normalize will only be called on a Matrix of type double */
	for (long i = 0; i < getLength(); i++)
	{
		double length = sqrt(matrix[0][i] * matrix[0][i] + matrix[1][i] * matrix[1][i] + matrix[2][i] * matrix[2][i]);	
		matrix[0][i] = matrix[0][i] / length;
		matrix[1][i] = matrix[1][i] / length;
		matrix[2][i] = matrix[2][i] / length;
	}

}

template <class T>
void Matrix<T>::print()
{
	ofstream output;
	output.open ("matrix_output.txt");
	for (long i = 0; i < getLength(); i++)
		output << std::scientific << matrix[0][i] << " " <<matrix[1][i] << " " << matrix[2][i] << endl;	
	output.close();
}
