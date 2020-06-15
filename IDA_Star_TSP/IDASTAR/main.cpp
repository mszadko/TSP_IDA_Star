#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <stack>

using namespace std;

const int WWA = 0;
const int LDZ = 1;
const int KAT = 2;
const int KRK = 3;
const int LBN = 4;
const int KIE = 5;
const int NumberOfCities = 6;

class SearchNode
{
public:
	SearchNode(int ID, int parentID)
	{
		cityID = ID;
		parentCityID = parentID;
	}
	bool Equals(SearchNode other)
	{
		return cityID == other.cityID;
	}
	bool Equals(int otherID)
	{
		return cityID == otherID;
	}
	int cityID;
	int parentCityID;
};

bool DoesElementExist(std::vector<SearchNode> vector, int data)
{
	for (std::vector<SearchNode>::iterator i = vector.begin(); i < vector.end(); i++)
	{
		if ((*i).cityID==data)
		{
			return true;
		}
	}
	return false;
}

int main()
{
	stack<SearchNode> DepthFirstTravelsalStack;
	vector<string> cities = { "Warszawa", "Lodz", "Katowice", "Krakow", "Lublin","Kielce" };
	int actions[NumberOfCities][NumberOfCities] =
	{
		{0, 135, 75, 300, 165, 180},
		{135, 0, 195, 270, 310, 140},
		{300, 195, 0, 75, 305, 180},
		{300, 270, 75, 0, 260, 120 },
		{165, 310, 305, 260, 0, 175},
		{180, 140, 180, 120, 175, 0}
	};

	int start = WWA;
	DepthFirstTravelsalStack.push(SearchNode(start, start));
	vector<SearchNode> visited;

	while (!DepthFirstTravelsalStack.empty())
	{
		//get new from stack
		SearchNode current = DepthFirstTravelsalStack.top();
		DepthFirstTravelsalStack.pop();		

		//check whether we make a return to some previous node in naive in depth search and make sure visited nodes are handled properly
		while (visited.size() >= 1 && current.parentCityID != visited[visited.size() - 1].cityID)
		{
			visited.pop_back();
		}
		
		//put new node on visited list
		visited.push_back(current);

		//add childs of current onto stack so we can visit them later.
		for (int i = NumberOfCities - 1; i >= 0; i--)
		{
			if (!DoesElementExist(visited, i) && actions[current.cityID][i] < INT_MAX)
			{
				DepthFirstTravelsalStack.push(SearchNode(i, current.cityID));
			}
		}

		//this is just a debug.
		if (visited.size() == NumberOfCities)
		{
			int length = 0;
			for (size_t i = 0; i < visited.size(); i++)
			{
				cout << cities[visited[i].cityID] << " -> ";
				length += actions[visited[i].parentCityID][visited[i].cityID];
			}
			cout <<"Warszawa\t\t Length = " << length + actions[visited[visited.size() - 1].cityID][start];
			cout << endl;
		}
	
		
	}
	return 0;
}
