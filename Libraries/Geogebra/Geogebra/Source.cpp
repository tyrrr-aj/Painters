#include <iostream>
#include <string>
#include <fstream>
#include <algorithm> 

#include "Point.h"
#include "Solver.h"

bool sortByOrderNumber(Point* a, Point* b)
{
	return (a->orderNumber < b->orderNumber);
}

void addPoint(std::vector<Point*>& points, double coordX, double coordY, int i)
{
	Point* point = new Point(coordX, coordY);
	points.push_back(point);
}

void readFile(std::vector<Point*>& points, std::string fileName)
{
	std::ifstream file;
	file.open(fileName);
	std::string line;
	int i = 0;
	while (getline(file,line))
	{
		std::string prefix = "\\rput";
		if (line.find(prefix) != std::string::npos)
		{
			size_t bracket1 = line.find("(");
			size_t comma = line.find(",");
			size_t bracket2 = line.find(")");

			std::string stringCoordX = line.substr(bracket1 + 1, comma - bracket1);
			std::string stringCoordY = line.substr(comma + 1, bracket2 - comma - 1);

			addPoint(points, std::stod(stringCoordX), std::stod(stringCoordY), i);
			++i;
		}
	}

	file.close();
}

int main()
{
	std::vector<Point*> points;
	readFile(points, "Geogebra.txt");
	int numberOfPts = points.size();

	Solver solver(points, 10, 10, numberOfPts);
	solver.solve();
	
	std::sort(points.begin(), points.end(), sortByOrderNumber);
	for (auto it = points.begin(); it != points.end(); ++it)
	{
		std::cout << (*it)->orderNumber <<": (" << (*it)->coordX << "," << (*it)->coordY << ")" << std::endl;
	}

	return 0;
	system("pause");
}