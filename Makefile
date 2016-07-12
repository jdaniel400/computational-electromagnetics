all: po
po: physicalOptics.o physics.o physics.hpp mesh.o mesh.hpp Matrix.o Matrix.hpp
	g++ -g physicalOptics.o physics.o mesh.o Matrix.o
physicalOptics.o: physicalOptics.cpp Matrix.hpp physics.hpp mesh.hpp
	g++ -g -c physicalOptics.cpp
physics.o: physics.cpp physics.hpp
	g++ -g -c physics.cpp
mesh.o: mesh.cpp mesh.hpp
	g++ -g -c mesh.cpp
Matrix.o: Matrix.cpp Matrix.hpp 
	g++ -g -c Matrix.cpp
clean:
	rm physicalOptics.o physics.o mesh.o Matrix.o
