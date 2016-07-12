all: po
po: physicalOptics.o physics.o mesh.o Matrix.o
	g++ -g physicalOptics.o physics.o mesh.o Matrix.o
physicalOptics.o: physicalOptics.cpp
	g++ -g -c physicalOptics.cpp -o physicalOptics.o
physics.o: physics.cpp physics.h
	g++ -g -c physics.cpp -o physics.o
mesh.o: mesh.cpp mesh.h
	g++ -g -c mesh.cpp -o mesh.o
Matrix.o: Matrix.cpp Matrix.h 
	g++ -g -c Matrix.cpp -o Matrix.o
clean:
	rm physicalOptics.o physics.o mesh.o Matrix.o
