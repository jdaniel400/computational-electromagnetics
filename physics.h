
/*All the methods that implement the physical optics approximation calculations */

Matrix<int> * calculateIlluminatedTriangles (Matrix<double> & torch, Matrix<double> & normals);

Matrix<complex<double> > * generateEFieldIncident (Matrix<int> * illuminated, Matrix<double> & polarizing_vector, Matrix<double> & torch, Matrix<double> * centroids);
