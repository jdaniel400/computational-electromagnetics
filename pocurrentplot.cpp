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
	long getLength();
	void normalize();
	void calculate_Centroids_and_Normals (Matrix & centroids, Matrix & normals, long num_triangles, Matrix & nodes, Matrix & triangles);
	Matrix * build_nodes (long num_columns, long start_of_node_field, long start_of_triangle_field);
	Matrix * build_triangles (long num_columns, long start_of_triangle_field, long end_of_triangle_field);



//private:
	T ** matrix; //Matrix class was templatized to support complex numbers
	long number_of_rows;
};

/*const double& Matrix::operator()(int row, int col) const
{
	return matrix[row][col]; //will return reference to cell entry, but cannot be modified
}
*/
template <class T>
T& Matrix<T>::operator()(long row, long col) const
{
	//note that it is ROW (Y) and then COL (X). MATLAB CONVENTION FORMAT
	return matrix[col][row]; //will return reference to cell entry, but can be modified
}

template <class T>
long Matrix<T>::getLength()
{
	return number_of_rows; //sizeof(matrix)/sizeof(matrix[0]);
}
template <class T>
Matrix<T> & Matrix<T>::cross (Matrix & arg_vect)
{
	/*Cross product implementation, iteratively for each row of the matrices
	assumes that template type supports * operator */
	Matrix<T> *result = new Matrix<T> (getLength(), 3);
	//This is the formula for cross product, implemented iteratively for each row of the matrices
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
 * 	Returns result as a double floating point*/

//	double * result = new double [getLength()];
//	for (int i = 0; i < getLength(); i++)
	return matrix[0][0] * arg_vect (0, 0) + matrix[1][0] * arg_vect (0, 1) + matrix[2][0] * arg_vect (0, 2);		
	
		
}
template <class T>
void Matrix<T>::normalize ()
{
	/* Assumes that normalize will only be called on a Matrix of type double */
	for (long i = 0; i < getLength(); i++)
	{
		double length = sqrt(matrix[0][i] * matrix[0][i] + matrix[1][i] * matrix[1][i] + matrix[2][i] * matrix[2][i]);	
		if (length == 0)
			cout << "error with 0 at" <<  i << ", check into " << endl;	
		matrix[0][i] = matrix[0][i] / length;
		matrix[1][i] = matrix[1][i] / length;
		matrix[2][i] = matrix[2][i] / length;
	}

}
void calculate_Centroids_and_Normals (Matrix<double> & centroids, Matrix<double> & normals, long num_triangles, Matrix<double> & nodes, Matrix<double> & triangles)
{
	Matrix<double> pts_A = *new Matrix<double> (num_triangles, 3); //RESOLVE: Possible memory leaks?????? Derefencing new object, calling copy constructor, then moving on?
	Matrix<double> pts_B = *new Matrix<double> (num_triangles, 3);
	Matrix<double> pts_C = *new Matrix<double> (num_triangles, 3);
	Matrix<double> vect1 = *new Matrix<double> (num_triangles, 3);
	Matrix<double> vect2 = *new Matrix<double> (num_triangles, 3);

	for (long i = 0; i < num_triangles; i++) {
		long nodeA = triangles(i,0); //triangles will always contain integers (in floating point format)
   		long nodeB = triangles(i,1);
    		long nodeC = triangles(i,2);
  	  	for (int j = 0; j < 3; j++)
    			pts_A (i, j) = nodes (nodeA, j); 
 	   	for (int j = 0; j < 3; j++)
    			pts_B (i, j) = nodes (nodeB, j);
    		for (int j = 0; j < 3; j++)
    			pts_C (i, j) = nodes (nodeC, j);
		for (int j = 0; j < 3; j++)
    			vect1(i, j) = nodes(nodeA, j) - nodes(nodeB, j);
	  	for (int j = 0; j < 3; j++)
    			vect2(i, j) = nodes(nodeA, j) - nodes(nodeC, j);

		//cout << "pts_A " << i << " " << pts_A (i, 0) << " " << pts_A (i, 1) << " " << pts_A (i, 2) << endl;
		//cout << "pts_B " << pts_B (i, 0) << " "<< pts_B (i, 1) << " " << pts_B (i, 2) << endl;
		//cout << "pts_C " << pts_C (i, 0) << " "<< pts_C (i, 1) << " " << pts_C (i, 2) << endl;
		
		centroids (i, 0) = (pts_A(i, 0) + pts_B(i, 0) + pts_C(i, 0)) / 3;
		centroids (i, 1) = (pts_A(i, 1) + pts_B(i, 1) + pts_C(i, 1)) / 3;
		centroids (i, 2) = (pts_A(i, 2) + pts_B(i, 2) + pts_C(i, 2)) / 3;
	
	}

	normals = vect1.cross(vect2);
	normals.normalize();

}
template <class T>
Matrix<T>::Matrix (const long num_rows, const long num_columns)
{
	matrix = new T*[num_columns];

	for (long i = 0; i < num_columns; i++)
		matrix[i] = new T [num_rows];

	for (long j = 0; j < num_columns; j++)
		for (long i = 0; i < num_rows; i++)
			matrix[j][i] = 0; //implemented this just because original MATLAB code called for zeros.
	number_of_rows = num_rows;
}


Matrix<double> * build_nodes (double * data, long start_of_node_field, long start_of_triangle_field)

{
	long prealloc_length = ((start_of_triangle_field - 3 - start_of_node_field) / 7) + 1; // added + 1 because originally MATLAB called for ceil
	Matrix<double> *tmp = new Matrix<double> (prealloc_length, 3);
	long i = start_of_node_field; // start i at 7
	int vals_taken = 0;
	long c = 0;
	long r = 0;
	while (i < start_of_triangle_field - 3) //if we opt for + 6 in main, this should change to - 8
	{ 
		if (c == 3) {
			c = 0;
			r++;
			i += 4; //skip ahead by 4, taken 3 coordinates.next 4 entires are irrelevant. Note that i is already pointing to an irrelevant entry, that's why it is 4 and not 5
		}
		(*tmp)(r, c) = data[i];
		//cout << "row " << r << " column " << c << "entry " << data[i] << endl;
		c ++;
		i ++;
	}
	return tmp;
}


Matrix<double> * build_triangles (double * data, long start_of_triangle_field, long end_of_triangle_field)
{
	//NOTE: Start of the triangle field has been designated as the first node number (with the code + 1) as of 6-29-16
	long prealloc_length = (end_of_triangle_field  - (start_of_triangle_field + 6))/9 + 1;
	Matrix<double> *tmp = new Matrix<double> ((prealloc_length), 3);
	long i = start_of_triangle_field + 6; //begin at the first node (this is dependent on the parsing process)
	int vals_taken = 0;
	long c = 0;
	long r = 0;
	while (i < end_of_triangle_field)
	{
  		if (c == 3)
  		{
 	    		c = 0;
 	    		r++;
			i += 6;
 		}
		(*tmp)(r, c) = data[i] - 1; //IMPORTANT: SUBTRACT 1. ALL ENTRIES IN TRIANGLE MATRIX REFER TO A OFFSET 1 TRIANGLE, NOT OFFSET 0 (THIS IS THE FILE CONVENTION)
 	   	c ++;
 	   	i ++;

       }	
	return tmp;
}

void getNewSubstring (char *& result, char * charArray, int start_index_inclusive, int end_index_exclusive)
{
	if (result != NULL) {
		delete[] result;
	//	result == NULL;
	}	
	result = new char [end_index_exclusive - start_index_inclusive + 1]; //+ 1 for null terminating char
	for (int x = start_index_inclusive; x < end_index_exclusive; x++)
		result[x - start_index_inclusive] = charArray[x];
	result[end_index_exclusive - start_index_inclusive] = '\0'; 
}

double * parseAndBuildData (const char *file_name, long & start_of_node_field, long & start_of_triangle_field, long & end_of_triangle_field) {
	ifstream mesh;
	mesh.open(file_name);
	long line_count = 0; //self explanatory, used in parsing file
	char *current_line = new char [MAX_CHARS_PER_LINE](); //the () is absolutely necessary to intialize the string to white space. Otherwise will be leftover values.
	long prev, cur; //WHAT IS CUR????
	double *current = new double [MAX_LINES]; //size of current is arbitrary
	bool SUBSTRING_EXISTS = false;
	long idx_for_current = 0;



	for (int i = 0; i < 185; i++)
		mesh.getline(current_line, MAX_CHARS_PER_LINE); // grab a few redundant characters. This isn't really necessary

	char * temp_ch_ar = NULL; //character array that holds the result of the number substring of a current line (eg "12e-3")

	while (mesh.getline(current_line, MAX_CHARS_PER_LINE)) 
	{
	//	cout << line_count << endl;
		int start_idx = 0;
		for (int i = 0; i < MAX_CHARS_PER_LINE; i++)
		{
			
			//I think that all the \0 s can go because every line ends with \r
			if (current_line[i] != ' ' && current_line[i] != '\r'&& current_line[i] != '\0')
				SUBSTRING_EXISTS = true; //flag true so that we can grab a substring next time a space comes up
			if (current_line[i] == 'D')
				current_line[i] = 'E'; //atoi recognizes E for floating point numbers, while text file contains D
			if ((current_line[i] == ' ' || current_line[i] == '\r' || current_line[i] == '\0') && SUBSTRING_EXISTS) {
				getNewSubstring(temp_ch_ar, current_line, start_idx, i);
				double tmp = atof(temp_ch_ar);
				//if (!(current_line[start_idx] == '0' && i == start_idx + 1) && tmp == 0) //basically check if atof returned 0 for an array other than '0'. This means it was an invalid number
				//		current[idx_for_current++] = -9999; //INVALID CHARACTER
			//	else
					current[idx_for_current++] = tmp;
				SUBSTRING_EXISTS = false; //reset flag to false
				start_idx = i + 1;
			}
			if (current_line[i] == '\r')
				break;
			else if (current_line[i] == ' ' ) //&& start_idx == i - 1)
				start_idx = i + 1;
		}

		
		line_count++;
		delete [] current_line;
		current_line = new char [MAX_CHARS_PER_LINE](); //must initialize, get rid of whatever is before

	}
	//	if (line_count == 2000) {
			//for (int i = 67300; i < 67395; i++)
		//		cout << "double " << i << " is: " << current[i] << endl;
	

	

	prev = current[0]; //prev variable used in detecting start field markers, initialize to first element
	for (long i = 0; i < idx_for_current; i++)
	{
		if (current[i] == 2411 && prev == -1) {
			start_of_node_field = i + 5;
			cout << "set at " << start_of_node_field << endl; }
		if (current[i] == 2412 && prev == -1)
			start_of_triangle_field = i + 1; 
		if (current[i] == 2420 && prev == -1)
			end_of_triangle_field = i - 2;

		prev = current[i];
	}
	cout << "start of node field " << current[start_of_node_field] << "(" << start_of_node_field << ")" << endl;
	cout << "start of triangle field " << current[start_of_triangle_field] << "(" << start_of_triangle_field << ")" << endl;
	cout << "end of triangle field " << current[end_of_triangle_field] << "(" << end_of_triangle_field << ")" << endl;
	mesh.close();
	return current;
}

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
int main ()
{
	
	
	
	long start_of_node_field, start_of_triangle_field, end_of_triangle_field; //markers filled in as part of the parsing process
	double  * data = parseAndBuildData ("sphere_veryhigh.txt", start_of_node_field, start_of_triangle_field, end_of_triangle_field);
	Matrix<double> *nodes = build_nodes (data, start_of_node_field, start_of_triangle_field); //yes, start_of_triangle_field is right here
	Matrix<double> *triangles = build_triangles (data, start_of_triangle_field, end_of_triangle_field);
	Matrix<double> *normals = new Matrix <double>(triangles->getLength(), 3);
	Matrix<double> *centroids = new Matrix <double>(triangles->getLength(), 3);
	calculate_Centroids_and_Normals (*centroids, *normals, triangles->getLength(), *nodes, *triangles);
	
	Matrix<double> torch = *new Matrix <double>(1, 3); //direction vector of incident E-field
	torch (0, 0) = 2; torch (0, 1) = 5; torch (0, 2) = 1; //completely arbitrary direction of the incident Electromagnetic field	
	
	Matrix<int> *illuminated = calculateIlluminatedTriangles (torch, *normals); //build the illuminated vector, binary representation of illuminated triangle/shadowed triangle for each triangle in the mesh
	
	Matrix<double> polarizing_vector = *new Matrix<double> (1, 3);
	polarizing_vector =  *new Matrix<double> (1,3);
	polarizing_vector (0,0) = 1; polarizing_vector (0,1) = 0; polarizing_vector (0, 2) = 0;
	
	Matrix <complex<double> >* precalc_exponentials = new Matrix <complex<double> >(normals->getLength(), 3);

	Matrix <complex<double> >* EFinc = generateEFieldIncident (illuminated, polarizing_vector, torch, centroids);
		
/*	for (int i = nodes->getLength() - 401; i < nodes->getLength(); i++)
		cout << "nodes" << i << ": " << (*nodes) (i, 0) << " " << (*nodes) (i, 1) << " " << (*nodes) (i, 2) << endl;
*/	
	//for (long i = triangles->getLength() - 31; i < triangles->getLength(); i++)
	//	cout << "normal " << i << ": " << (*normals)(i, 0) << " " << (*normals)(i, 1) << " " << (*normals)(i, 2) << endl;

	for (int i = EFinc->getLength() - 310; i < EFinc->getLength(); i++)
		cout << "E field inc " << i << ": " << (*EFinc) (i, 0).real() << " + " << (*EFinc) (i, 0).imag() << "i " << (*EFinc) (i, 1).real() << " + " << (*EFinc) (i, 1).imag() << "i " << (*EFinc) (i, 2).real() << " + " << (*EFinc) (i, 2).imag() << "i " << endl;
	
	/*for (int i = centroids->getLength() - 250; i < centroids->getLength(); i++)
		cout << "centroid " << i << ": " << (*illuminated)(i, 0) << endl ; //" " << (*centroids)(i, 1) << " " << (*centroids)(i, 2) << endl;
*/
	
	return 0;
}

