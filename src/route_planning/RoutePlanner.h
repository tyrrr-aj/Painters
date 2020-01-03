#include <iostream>
#include <string>
#include <algorithm> 
#include <cstdlib>

#include "RpPoint.h"
#include "Solver.h"
#include "../file_loader/file_loader.h"
#include "../geometry/Point.h"

class RoutePlanner {
	public:
		RoutePlanner(FileLoader* fileLoader);
		std::vector<Point*> getPath(std::string);
	
	private:
		FileLoader* fileLoader;
		void readFile(std::vector<RpPoint*>&, std::string);
		void addPoint(std::vector<RpPoint*>&, double, double, int);
		static bool sortByOrderNumber(RpPoint*, RpPoint*);
};