#include <iostream>
#include <string>
#include <fstream>
#include <algorithm> 
#include <cstdlib>

#include "Point.h"
#include "Solver.h"

class RoutePlanner {
	public:
		std::vector<Point> getPath(std::string);
	
	private:
		void readFile(std::vector<Point*>&, std::string);
		void addPoint(std::vector<Point*>&, double, double, int);
		bool sortByOrderNumber(Point*, Point*);
};