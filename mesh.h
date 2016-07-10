
/*All the functions necessary to build the mesh from the .unv file */


void calculate_Centroids_and_Normals (Matrix<double> & centroids, Matrix<double> & normals, long num_triangles, Matrix<double> & nodes, Matrix<double> & triangles);

Matrix<double> * build_nodes (double * data, long start_of_node_field, long start_of_triangle_field);

Matrix<double> * build_triangles (double * data, long start_of_triangle_field, long end_of_triangle_field);

void getNewSubstring (char *& result, char * charArray, int start_index_inclusive, int end_index_exclusive);

double * parseAndBuildData (const char *file_name, long & start_of_node_field, long & start_of_triangle_field, long & end_of_triangle_field);

