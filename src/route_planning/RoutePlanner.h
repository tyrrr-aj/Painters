#include <iostream>
#include <string>
#include <algorithm> 
#include <cstdlib>

#include "Point.h"
#include "Solver.h"
#include "../file_loader/file_loader.h"

class RoutePlanner {
	public:
		RoutePlanner(FileLoader* fileLoader);
		std::vector<Point*> getPath(std::string);
	
	private:
		FileLoader* fileLoader;
		void readFile(std::vector<Point*>&, std::string);
		void addPoint(std::vector<Point*>&, double, double, int);
		static bool sortByOrderNumber(Point*, Point*);
};