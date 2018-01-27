all: hakoiri

hakoiri: hakoiri.cpp
	g++ -std=c++11 $< -o $@
