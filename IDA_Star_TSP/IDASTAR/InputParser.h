#pragma once
#include <vector>
#include <string>

 

class InputParser
{
public:
	static bool ParseData(const std::string dataFile, std::vector<std::vector<int>>& actions, std::vector<std::string>& cities, size_t* NumberOfCities);
	
};

