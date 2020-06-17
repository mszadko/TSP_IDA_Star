#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <math.h>

using namespace std;

struct City
{
	City() :City(0, 0)
	{}
	City(int x_, int y_)
	{
		x = x_;
		y = y_;
	}
	int x, y;
};


size_t NumberOfCities;
const string dataFile="dj38.txt";
vector<vector<int>>actions;
vector<City> cities;

int euclidian(City a, City b)
{
	int xDiff = abs(a.x - b.x);
	int yDiff = abs(a.y - b.y);
	return (int)sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}



int main()
{
	std::ifstream dataStream;
	dataStream.open(dataFile);
	if (!dataStream)
	{
		std::cout << "There was problem with file at " << dataFile << "\nPlease make sure the path is correct.\n";
	}
	try
	{
		//get first line - should be a single int number with number of cities
		std::string line;
		getline(dataStream, line);
		std::istringstream ss(line);
		ss >> NumberOfCities;


		//alocate space for actions
		actions.clear();
		for (size_t i = 0; i < NumberOfCities; i++)
		{
			actions.push_back(std::vector<int>());
			for (size_t j = 0; j < NumberOfCities; j++)
			{
				actions[i].push_back(0);
			}
		}

		for (size_t i = 0; i < NumberOfCities; i++)
		{
			getline(dataStream, line, ' ');
			getline(dataStream, line, '.');
			ss = istringstream(line);
			int x;
			ss >> x;
			getline(dataStream, line, ' ');
			getline(dataStream, line, '.');
			ss = istringstream(line);
			int y;
			ss >> y;
			cities.push_back(City(x, y));
		}

		//lets normalize cities coordinates to make sure it fits in integers
		int minX=INT_MAX;
		int minY= INT_MAX;

		for (size_t i = 0; i < cities.size(); i++)
		{
			if (cities[i].x<minX)
			{
				minX = cities[i].x;
			}
			if (cities[i].y < minY)
			{
				minY = cities[i].y;
			}
		}

		for (size_t i = 0; i < cities.size(); i++)
		{
			cities[i].x -= minX;
			cities[i].y -= minY;
		}
		for (size_t i = 0; i < cities.size(); i++)
		{
			cout << "(" << cities[i].x << ", " << cities[i].y << ")\n";
		}
		ofstream result;
		result.open("dj38parsed.txt");
		result << NumberOfCities << endl;
		for (size_t i = 0; i < NumberOfCities; i++)
		{
			result <<"citynum"<< i << ';';
		}
		result << endl;
		for (size_t i = 0; i < NumberOfCities; i++)
		{
			actions.push_back(std::vector<int>());
			for (size_t j = 0; j < NumberOfCities; j++)
			{
				actions[i][j] = euclidian(cities[i], cities[j]);
				result << actions[i][j] << ';';
			}
			result << endl;
		}
		dataStream.close();
		result.close();
	}
	catch (const std::exception&)
	{
		std::cout << "There was problem with file at " << dataFile << "\nPlease make sure that data is correctly formated.\n";
	}
	return 0;
}