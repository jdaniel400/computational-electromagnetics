#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <fstream>

#define MAX_CHARS_PER_LINE 93
using namespace std;

class Matrix
{
public:
	Matrix (const int num_rows, const int num_columns);
	//const double& operator()(int row, int col) const;
	Matrix & cross (Matrix & arg_vect);
	double& operator()(int row, int col) const;
	int getLength();
	void normalize();
	void calculate_Centroids_and_Normals (Matrix & centroids, Matrix & normals, int num_triangles, Matrix & nodes, Matrix & triangles);
	Matrix * build_nodes (int num_columns, int start_of_node_field, int start_of_triangle_field);
	Matrix * build_triangles (int num_columns, int start_of_triangle_field, int end_of_triangle_field);



private:
	double ** matrix;
};

/*const double& Matrix::operator()(int row, int col) const
{
	return matrix[row][col]; //will return reference to cell entry, but cannot be modified
}
*/

double& Matrix::operator()(int row, int col) const
{
	//note that it is ROW (Y) and then COL (X). MATLAB CONVENTION FORMAT
	return matrix[col][row]; //will return reference to cell entry, but can be modified
}

int Matrix::getLength()
{
	cout << "size is " << sizeof(matrix)/sizeof(matrix[0]);
	return sizeof(matrix)/sizeof(matrix[0]);
}

Matrix & Matrix::cross (Matrix & arg_vect)
{
	Matrix *result = new Matrix (getLength(), 3);
	for (int i = 0; i < getLength(); i++)
	{
		(*result)(i,1) = matrix[i][2] * arg_vect(i,3) - matrix[i][3] * arg_vect(i,2);
		(*result)(i,2) = matrix[i][3] * arg_vect(i,1) - matrix[i][1] * arg_vect(i,3);
		(*result)(i,3) = matrix[i][1] * arg_vect(i,2) - matrix[i][2] * arg_vect(i,1);
	}
	return *result;
}

void Matrix::normalize ()
{
	for (int i = 0; i < getLength(); i++)
	{
		double length = sqrt(matrix[i][1] * matrix[i][1] + matrix[i][2] * matrix[i][2] + matrix[i][3] * matrix[i][3]);	
		matrix[i][1] = matrix[i][1] / length;
		matrix[i][2] = matrix[i][2] / length;
		matrix[i][3] = matrix[i][3] / length;
	}

}
void calculate_Centroids_and_Normals (Matrix & centroids, Matrix & normals, int num_triangles, Matrix & nodes, Matrix & triangles)
{
	Matrix pts_A = *new Matrix (num_triangles, 3);
	Matrix pts_B = *new Matrix (num_triangles, 3);
	Matrix pts_C = *new Matrix (num_triangles, 3);
	Matrix vect1 = *new Matrix (num_triangles, 3);
	Matrix vect2 = *new Matrix (num_triangles, 3);

	for (int i = 0; i < num_triangles; i++) {
		int nodeA = triangles(i,1);
   		int nodeB = triangles(i,2);
    	int nodeC = triangles(i,3);
    	for (int j = 0; j < 3; j++)
    		pts_A (i, j) = nodes (nodeA, j);

    	for (int j = 0; j < 3; j++)
    		pts_B (i, j) = nodes (nodeB, j);
    	for (int j = 0; j < 3; j++)
    		pts_C (i, j) = nodes (nodeC, j);
    	for (int j = 0; j < 3; j++)
    		vect1(i, j) = nodes(nodeA, j) - nodes(nodeB, j);
    	for (int j = 0; j < 3; j++)
    		vect2(i, 1) = nodes(nodeA, j) - nodes(nodeC, j);
	

		centroids (i, 1) = (pts_A(i, 1) + pts_B(i, 1) + pts_C(i, 1)) / 3;
		centroids (i, 2) = (pts_A(i, 2) + pts_B(i, 2) + pts_C(i, 2)) / 3;
		centroids (i, 3) = (pts_A(i, 3) + pts_B(i, 3) + pts_C(i, 3)) / 3;
	
	}

	normals = vect1.cross(vect2);
	normals.normalize();

}

Matrix::Matrix (const int num_rows, const int num_columns)
{
	matrix = new double*[num_columns];

	for (int i = 0; i < num_columns; i++)
		matrix[i] = new double [num_rows];

	for (int j = 0; j < num_columns; j++)
		for (int i = 0; i < num_rows; i++)
			matrix[j][i] = 0; //implemented this just because original MATLAB code called for zeros.
}


Matrix * build_nodes (double * data, int start_of_node_field, int start_of_triangle_field)

{
	int prealloc_length = ((start_of_triangle_field - 3 - start_of_node_field) / 7) + 1; // added + 1 because originally MATLAB called for ceil
	Matrix *tmp = new Matrix (prealloc_length, 3);
	int i = start_of_node_field; // start i at 7
	int vals_taken = 0;
	int c = 0;
	int r = 0;
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


Matrix * build_triangles (double * data, int start_of_triangle_field, int end_of_triangle_field)
{
	//NOTE: Start of the triangle field has been designated as the first node number (with the code + 1) as of 6-29-16
	int prealloc_length = (end_of_triangle_field  - (start_of_triangle_field + 6))/9 + 1;
	Matrix *tmp = new Matrix ((prealloc_length), 3);
	int i = start_of_triangle_field + 6; //begin at the first node (this is dependent on the parsing process)
	int vals_taken = 0;
	int c = 0;
	int r = 0;
	while (i < end_of_triangle_field)
	{
  		if (c == 3)
  		{
 	    		c = 0;
 	    		r++;
			i += 6;
 		}
		(*tmp)(r, c) = data[i];
		//cout << "row " << r << " column " << c << "entry " << data[i] << endl;
 	   	c ++;
 	   	i ++;

       }	
	return tmp;
}

void getNewSubstring (char *& result, char * charArray, int start_index_inclusive, int end_index_exclusive)
{
	
/*	char * newString = new char [(end_index_exclusive - 1) - start_index_inclusive];
	for (int x = 0; x < end_index_exclusive; x++)
		newString[x] = charArray[x];
	result = newString;
	//return newString;
*/	if (start_index_inclusive == end_index_exclusive) {
		cout << "tried to print a substring of size zero at line " << charArray << " but only wanted: '" << charArray[start_index_inclusive] << "'" << endl;
		return;
	}
	if (result != NULL)
		delete[] result;	
	result = new char [end_index_exclusive - start_index_inclusive + 1]; //+ 1 for null terminating char
	for (int x = start_index_inclusive; x < end_index_exclusive; x++)
		result[x - start_index_inclusive] = charArray[x];
	result[end_index_exclusive - start_index_inclusive] = '\0'; 
	//result = newString;
	//return newString;
}

int main ()
{
	ifstream mesh;
	mesh.open("shape.txt");
	int start_of_node_field, start_of_triangle_field, end_of_triangle_field; //markers filled in as part of the parsing process
	int line_count = 0; //self explanatory, used in parsing file
	char *current_line = new char [MAX_CHARS_PER_LINE](); //the () is absolutely necessary to intialize the string to white space. Otherwise will be leftover values.
	int prev, cur; //WHAT IS CUR????
	double current [80000]; //size of current is arbitrary
	bool SUBSTRING_EXISTS = false;
	int idx_for_current = 0;


	for (int i = 0; i < 185; i++)
		mesh.getline(current_line, MAX_CHARS_PER_LINE); // grab a few redundant characters. This isn't really necessary

	char * temp_ch_ar = NULL; //character array that holds the result of the number substring of a current line (eg "12e-3")

	while (mesh.getline(current_line, MAX_CHARS_PER_LINE)) 
	{
		cout << line_count << endl;
		int start_idx = 0;
		//cout << "LOOK AT THIS " << endl;
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
				if (!(current_line[start_idx] == '0' && i == start_idx + 1) && tmp == 0) //basically check if atof returned 0 for an array other than '0'. This means it was an invalid number
						current[idx_for_current++] = -9999; //INVALID CHARACTER
				else
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
			for (int i = 67300; i < 67395; i++)
				cout << "double " << i << " is: " << current[i] << endl;
	

	

	prev = current[0]; //prev variable used in detecting start field markers, initialize to first element
	for (int i = 0; i < 80000; i++)
	{
		if (current[i] == 2411 && prev == -1)
			start_of_node_field = i + 5;
		if (current[i] == 2412 && prev == -1)
			start_of_triangle_field = i + 1; // + 6 ?????? for the bottom 3 it should be + 6
		if (current[i] == 2420 && prev == -1)
			end_of_triangle_field = i - 2;

		prev = current[i];
	}
	cout << "start of node field " << current[start_of_node_field] << "(" << start_of_node_field << ")" << endl;
	cout << "start of triangle field " << current[start_of_triangle_field] << "(" << start_of_triangle_field << ")" << endl;
	cout << "end of triangle field " << current[end_of_triangle_field] << "(" << end_of_triangle_field << ")" << endl;
	
	
	
	Matrix *nodes = build_nodes (current, start_of_node_field, start_of_triangle_field); //yes, start_of_triangle_field is right here
	Matrix *triangles = build_triangles (current, start_of_triangle_field, end_of_triangle_field);
	Matrix *normals = new Matrix (nodes->getLength(), 3);
	Matrix *centroids = new Matrix (nodes->getLength(), 3);
	calculate_Centroids_and_Normals (*centroids, *normals, triangles->getLength(), *nodes, *triangles);
	mesh.close();
	return 0;
}

