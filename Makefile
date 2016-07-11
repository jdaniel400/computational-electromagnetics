all: po
po: physicalOptics.o physics.o mesh.o Matrix.o
	g++ -g physicalOptics.o physics.o mesh.o Matrix.o
physicalOptics.o: physicalOptics.cpp
	g++ -g -c physicalOptics.cpp -o physicalOptics.o
physics.o: physics.cpp
	g++ -g -c physics.cpp -o physics.o
mesh.o: mesh.cpp 
	g++ -g -c mesh.cpp -o mesh.o
Matrix.o: Matrix.cpp 
	g++ -g -c Matrix.cpp -o Matrix.o
clean:
	rm physicalOptics.o physics.o mesh.o Matrix.o
