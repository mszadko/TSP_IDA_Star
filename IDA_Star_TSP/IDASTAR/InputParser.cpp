#include "InputParser.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>


bool InputParser::ParseData(const std::string dataFile, std::vector<std::vector<int>>& actions,std::vector<std::string>& cities,size_t* NumberOfCities)
{
	std::ifstream dataStream;
	dataStream.open(dataFile);
	if (!dataStream)
	{
		std::cout << "There was problem with file at " << dataFile << "\nPlease make sure the path is correct.\n";
		return false;
	}
	try
	{
		//get first line - should be a single int number with number of cities
		std::string line;
		getline(dataStream, line);
		std::istringstream ss(line);
		ss >> *NumberOfCities;


		actions.clear();
		for (size_t i = 0; i < *NumberOfCities; i++)
		{
			actions.push_back(std::vector<int>());
			for (size_t j = 0; j < *NumberOfCities; j++)
			{
				actions[i].push_back(0);
			}
		}

		//get 2nd line - should be city names separated by ';'
		getline(dataStream, line);
		ss = std::istringstream(line);
		cities.clear();
		std::string city;
		while (getline(ss, city, ';'))
		{
			cities.push_back(city);
		}

		//get rest of text file. it should be NumberOfCities x NumberOfCities matrix with action costs that take you from i-th city to j-th city.
		int index = 0;
		while (getline(dataStream, line, ';'))
		{
			std::istringstream ss(line);
			int x;
			ss >> x;
			actions[index / *NumberOfCities][index % *NumberOfCities] = x;
			index++;
		}
		return true;
	}
	catch (const std::exception&)
	{
		std::cout << "There was problem with file at " << dataFile << "\nPlease make sure that data is correctly formated.\n";
		return false;
	}
}
