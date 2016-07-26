#include "mesh.hpp"
#include "Matrix.cpp"

Mesh::Mesh ()
{
	data = parseAndBuildData ("mesh/sphere_veryhigh.txt");
	
	nodes = build_nodes (); //yes, start_of_triangle_field is right here
	triangles = build_triangles ();
	normals = new Matrix <double>(triangles->getLength(), 3);
	centroids = new Matrix <double>(triangles->getLength(), 3);
	calculate_Centroids_and_Normals ();
}
void Mesh::calculate_Centroids_and_Normals ()
{
	long num_triangles = triangles->getLength();
	Matrix<double> pts_A = *new Matrix<double> (num_triangles, 3); //RESOLVE: Possible memory leaks?????? Derefencing new object, calling copy constructor, then moving on?
	Matrix<double> pts_B = *new Matrix<double> (num_triangles, 3);
	Matrix<double> pts_C = *new Matrix<double> (num_triangles, 3);
	Matrix<double> vect1 = *new Matrix<double> (num_triangles, 3);
	Matrix<double> vect2 = *new Matrix<double> (num_triangles, 3);

	for (long i = 0; i < num_triangles; i++) {
		long nodeA = (*triangles)(i,0); //triangles will always contain integers (in floating point format)
   		long nodeB = (*triangles)(i,1);
    		long nodeC = (*triangles)(i,2);
  	  	for (int j = 0; j < 3; j++)
    			pts_A (i, j) = (*nodes) (nodeA, j); 
 	   	for (int j = 0; j < 3; j++)
    			pts_B (i, j) = (*nodes) (nodeB, j);
    		for (int j = 0; j < 3; j++)
    			pts_C (i, j) = (*nodes) (nodeC, j);
		for (int j = 0; j < 3; j++)
    			vect1(i, j) = (*nodes) (nodeA, j) - (*nodes)(nodeB, j);
	  	for (int j = 0; j < 3; j++)
    			vect2(i, j) = (*nodes) (nodeA, j) - (*nodes)(nodeC, j);

		//cout << "pts_A " << i << " " << pts_A (i, 0) << " " << pts_A (i, 1) << " " << pts_A (i, 2) << endl;
		//cout << "pts_B " << pts_B (i, 0) << " "<< pts_B (i, 1) << " " << pts_B (i, 2) << endl;
		//cout << "pts_C " << pts_C (i, 0) << " "<< pts_C (i, 1) << " " << pts_C (i, 2) << endl;
		
		(*centroids) (i, 0) = (pts_A(i, 0) + pts_B(i, 0) + pts_C(i, 0)) / 3;
		(*centroids) (i, 1) = (pts_A(i, 1) + pts_B(i, 1) + pts_C(i, 1)) / 3;
		(*centroids) (i, 2) = (pts_A(i, 2) + pts_B(i, 2) + pts_C(i, 2)) / 3;
	
	}

	normals = &(vect1.cross(vect2));
	normals->normalize();

}

Matrix<double> * Mesh::build_nodes ()

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


Matrix<double> * Mesh::build_triangles ()
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

void Mesh::getNewSubstring (char *& result, char * charArray, int start_index_inclusive, int end_index_exclusive)
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

double * Mesh::parseAndBuildData (const char *file_name)
{
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
