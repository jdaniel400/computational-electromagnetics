all: po
po: physicalOptics.o physics.o src/physics.hpp mesh.o src/mesh.hpp Matrix.o src/Matrix.hpp
	g++ -g physicalOptics.o physics.o mesh.o Matrix.o -o output/PO
physicalOptics.o: src/physicalOptics.cpp src/Matrix.hpp src/physics.hpp src/mesh.hpp
	g++ -g -c src/physicalOptics.cpp
physics.o: src/physics.cpp src/physics.hpp
	g++ -g -c src/physics.cpp
mesh.o: src/mesh.cpp src/mesh.hpp
	g++ -g -c src/mesh.cpp
Matrix.o: src/Matrix.cpp src/Matrix.hpp 
	g++ -g -c src/Matrix.cpp
clean:
	rm physicalOptics.o physics.o mesh.o Matrix.o
