CXX = g++

all:
	$(CXX) -o spinning_cube spinning_cube.cpp -std=c++17

clean:
	rm -f spinning_cube