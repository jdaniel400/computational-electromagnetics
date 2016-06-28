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
	return matrix[row][col]; //will return reference to cell entry, but can be modified
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

	for (int i = 0; i < num_rows; i++)
		matrix[i] = new double [num_rows];

	for (int j = 0; j < num_rows; j++)
		for (int i = 0; i < num_columns; i++)
			matrix[i][j] = 0; //implemented this just because original MATLAB code called for zeros
}


Matrix * build_nodes (int num_columns, int start_of_node_field, int start_of_triangle_field)
{
	ifstream mesh;
	mesh.open("shape.txt");
	char buf [MAX_CHARS_PER_LINE];
	for (int idx = 0; idx < start_of_node_field; idx++)
		mesh.getline(buf, MAX_CHARS_PER_LINE); //don't need to go anywhere
	int prealloc_length = ((start_of_triangle_field - 2 - start_of_node_field) / 7) + 1; // added + 1 because originally MATLAB called for ceil
	Matrix *tmp = new Matrix (prealloc_length, num_columns);
	int i = start_of_node_field; // start i at 7
	int vals_taken = 0;
	int c = 0;
	int r = 0;
	while (i < start_of_triangle_field - 2)
	{ 
		if (c == 3) {
			c = 1;
			r += 1;
		}
		mesh.getline(buf, MAX_CHARS_PER_LINE); //later getline
		(*tmp)(r, c) = atof(buf);
		c += 1;
		vals_taken = vals_taken + 1;
		i += 1;
		if (vals_taken == 3) {
			i += 4; //kip ahead by 4, taken 3 values (vertexes) already. Don't care about the next 3 or 4?? entries
			vals_taken = 0;
		}
	}
	return tmp;
}


Matrix * build_triangles (int num_columns, int start_of_triangle_field, int end_of_triangle_field)
{
	//BE CAREFUL WITH THE 0 1 C++ MATLAB ARRAY START INDEX MISMATCH
	ifstream mesh;
	mesh.open("shape.txt");
	char buf [MAX_CHARS_PER_LINE];
	for (int idx = 0; idx < start_of_triangle_field + 6; idx++)
		mesh.getline(buf, MAX_CHARS_PER_LINE);
	int prealloc_length = (end_of_triangle_field  - (start_of_triangle_field + 6))/9 + 1;
	Matrix *tmp = new Matrix ((prealloc_length), num_columns);
	int i = start_of_triangle_field + 6; // start i at 7
	int vals_taken = 0;
	int c = 0;
	int r = 0;
	while (i < end_of_triangle_field)
	{
  		if (c == 3)
  		{
 	    	c = 0;
 	    	r = r + 1;
 		}
    
		mesh.getline(buf, MAX_CHARS_PER_LINE); //later getline
		(*tmp)(r, c) = atof(buf);

 	   c += 1;
 	   vals_taken = vals_taken + 1;
 	   i += 1;
 	   if (vals_taken == 3)
 	   {
    	   i += 6; 
    	   vals_taken = 0; 	
       }
	}
	return tmp;
}

char * getNewSubstring (char * charArray, int start_index_inclusive, int end_index_exclusive)
{
	char * newString = new char [(end_index_exclusive - 1) - start_index_inclusive];
	for (int x = 0; x < end_index_exclusive; x++)
		newString[x] = charArray[x];
	return newString;
}
int main ()
{
	ifstream mesh;
	mesh.open("shape.txt");
	int start_of_node_field, start_of_triangle_field, end_of_triangle_field;
	int line_count = 0;
	char *current_line = new char [MAX_CHARS_PER_LINE](); //the () is absolutely necessary to intialize the string to white space. Otherwise will be leftover values.
	int prev, cur;
	double current [50000];
	bool SUBSTRING_EXISTS = false;
	int idx_for_current = 0;
	bool delete_this_flag;

/*
	for (int i = 0; i < 8000; i++)
		mesh.getline(current_line, MAX_CHARS_PER_LINE);
*/

	while (mesh.getline(current_line, MAX_CHARS_PER_LINE)) 
	{
		int start_idx = 0;
		//cout << "LOOK AT THIS " << endl;
		for (int i = 0; i < MAX_CHARS_PER_LINE; i++)
		{
			//cout << line_count << endl;
			if (line_count == 1169) {
				cout << current_line[i]; //current_line[i];
			}
			//I think that all the \0 s can go because every line ends with \r
			if (current_line[i] == '0') 
				delete_this_flag = true; //breakpoint statement
			if (current_line[i] != ' ' && current_line[i] != '\r'&& current_line[i] != '\0')
				SUBSTRING_EXISTS = true; //flag true so that we can grab a substring next time a space comes up
			
			if ((current_line[i] == ' ' || current_line[i] == '\r' || current_line[i] == '\0') && SUBSTRING_EXISTS) {
				char *temp_ch_ar = getNewSubstring(current_line, start_idx, i);
				double tmp = atof(getNewSubstring(current_line, start_idx, i));
				delete temp_ch_ar;
				if (!(current_line[start_idx] == '0' && i == start_idx + 1) && tmp == 0) //basically check if atof returned 0 for an array other than '0'. This means it was an invalid number
						current[idx_for_current++] = -9876543210;
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

	/*	
		if (line_count == 2000) {
			for (int i = 0; i < 2000; i++)
				cout << "double " << i << " is: " << current[i] << endl;
		}
*/
		line_count++;
		delete current_line;
		current_line = new char [MAX_CHARS_PER_LINE](); //must initialize, get rid of whatever is before

	}
	cout << " SALL GOOD MAN" << endl;
	for (int i = 0; i < 50000; i++)
	{
		if (current[i] == 2411 && prev == -1)
			start_of_node_field = line_count + 5;
		if (cur == 2412 && prev == -1)
			start_of_triangle_field = line_count + 1; 
		if (cur == 2420 && prev == -1)
			end_of_triangle_field = line_count - 2;


	}


		//if (line_count > 76) break;
		cur = atoi(current_line);
		if (cur == 2411 && prev == -1)
			start_of_node_field = line_count + 5;
		if (cur == 2412 && prev == -1)
			start_of_triangle_field = line_count + 1; 
		if (cur == 2420 && prev == -1)
			end_of_triangle_field = line_count - 2;


		//I chose to exclude the matlab safeguard for smaller meshes and triangle field, because I think that is has been made obsolete
		prev = cur;
		for (int z = 0; z < MAX_CHARS_PER_LINE; z++) {
	//		cout << current_line[z];
		} 

	

	Matrix *nodes = build_nodes (3, start_of_node_field, start_of_triangle_field); //yes, start_of_triangle_field is right here
	Matrix *triangles = build_triangles (3,start_of_triangle_field, end_of_triangle_field);
	Matrix *normals = new Matrix (nodes->getLength(), 3);
	Matrix *centroids = new Matrix (nodes->getLength(), 3);
	calculate_Centroids_and_Normals (*centroids, *normals, triangles->getLength(), *nodes, *triangles);
	mesh.close();
}

