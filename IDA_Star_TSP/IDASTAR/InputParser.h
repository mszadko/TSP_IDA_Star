#pragma once
#include <vector>
#include <string>

 
class Node;


class InputParser
{
public:
	static void Parse(std::string filename, std::vector<Node>& nodes);
};

