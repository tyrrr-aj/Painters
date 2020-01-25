#include "RoutePlanner.h"

RoutePlanner::RoutePlanner(FileLoader* fileLoader) {
	this->fileLoader = fileLoader;
}

bool RoutePlanner::sortByOrderNumber(RpPoint* a, RpPoint* b)
{
	return (a->orderNumber < b->orderNumber);
}

void RoutePlanner::addPoint(std::vector<RpPoint*>& points, double coordX, double coordY, int i)
{
	RpPoint* point = new RpPoint(coordX, coordY);
	points.push_back(point);
}

void RoutePlanner::readFile(std::vector<RpPoint*>& points, std::string fileName)
{
	File file;
	file = fileLoader->loadFile(fileName);
	std::string line;
	int i = 0;
	while (fileLoader->getLine(file, &line))
	{
		std::string prefix = "\\rput";
		if (line.find(prefix) != std::string::npos)
		{
			size_t bracket1 = line.find("(");
			size_t comma = line.find(",");
			size_t bracket2 = line.find(")");

			std::string stringCoordX = line.substr(bracket1 + 1, comma - bracket1);
			std::string stringCoordY = line.substr(comma + 1, bracket2 - comma - 1);

			addPoint(points, std::strtod(stringCoordX.c_str(), NULL), std::strtod(stringCoordY.c_str(), NULL), i);
			++i;
		}
	}
	fileLoader->closeFile(file);
}

std::vector<Point*> RoutePlanner::getPath(std::string path)
{
	std::vector<RpPoint*> points;
	readFile(points, path);
	int numberOfPts = points.size();
	
	Solver solver(points, 10, 10, numberOfPts);
	solver.solve();
	
	std::sort(points.begin(), points.end(), sortByOrderNumber);

	// TODO: quick fix below, properly RpPoint should inherit from Point and be directly returned as Point
	std::vector<Point*> output;
	for (int i = 0; i < points.size(); i++) {
		output.push_back(new Point(points[i]->coordX, points[i]->coordY));
	}
	return output;
/*	for (auto it = points.begin(); it != points.end(); ++it)
	{
		std::cout << (*it)->orderNumber <<": (" << (*it)->coordX << "," << (*it)->coordY << ")" << std::endl;
	}

	Point startingPoint = *points[0];

	//Chassis chassis(startingPoint, );
	//Steering steering(points,)

	return 0;
	system("pause");*/
}