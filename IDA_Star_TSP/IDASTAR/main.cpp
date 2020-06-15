#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <stack>

using namespace std;
vector<string> cities = { "Warszawa", "Lodz", "Katowice", "Krakow", "Lublin","Kielce" };
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

int h1(vector<SearchNode> visited, vector<int> unvisited, int actions[NumberOfCities][NumberOfCities], int startNode)
{
	if (unvisited.size() == 0)
		return 0;
	int h1 = 0;
	int f = 0, t = 0;
	int shortestConnection = INT_MAX;
	//look for shortest way to connect visited cities with unvisited ones
	for (size_t i = 0; i < visited.size(); i++)
	{
		int from = visited[i].cityID;
		for (size_t j = 0; j < unvisited.size(); j++)
		{
			int to = unvisited[j];
			if (from!=to&&actions[from][to]<INT_MAX)
			{
				if (actions[from][to]<shortestConnection)
				{
					shortestConnection = actions[from][to];
					f = from;
					t = to;
				}
			}
		}
	}
	cout << "Connection from visited to unvisited = " << shortestConnection << " from " << cities[f]<< " to " << cities[t] << endl;
	h1 = shortestConnection;
	f = 0; t = 0;
	//now lets find cheapest action that take us from every unvisited node to another unvisited node or to the start node
	for (size_t i = 0; i < unvisited.size(); i++)
	{
		int shortestWayOut = INT_MAX;
		int from = unvisited[i];
		for (size_t j = 0; j < unvisited.size(); j++)
		{
			int to = unvisited[j];
			if (from != to && actions[from][to] < INT_MAX)
			{
				if (actions[from][to] < shortestWayOut)
				{
					shortestWayOut = actions[from][to];
					f = from;
					t = to;
				}
				//we additionaly need to check way to start node
				if (j == unvisited.size() - 1)
				{
					if (actions[from][startNode] < shortestWayOut)
					{
						shortestWayOut = actions[from][startNode];
						f = from;
						t = startNode;
					}
				}
			}
		}
		if (f != t)
		{
			h1 += shortestWayOut;
			cout << "Shortest way out from " << cities[f] << " to  " << cities[t] << "  costs " << shortestWayOut << endl;
		}
	}
	return h1;
}

int main()
{
	stack<SearchNode> DepthFirstTravelsalStack;
	//  0-Warszawa , 1-Lodz, 2-Katowice , 3-Krakow, 4-Lublin, 5-Kielce 
	int actions[NumberOfCities][NumberOfCities] =
	{
		{0, 135, 300, 300, 165, 180},
		{135, 0, 195, 270, 310, 140},
		{300, 195, 0, 75, 305, 180},
		{300, 270, 75, 0, 260, 120 },
		{165, 310, 305, 260, 0, 175},
		{180, 140, 180, 120, 175, 0}
	};



	int start = WWA;
	DepthFirstTravelsalStack.push(SearchNode(start, start));
	vector<SearchNode> visited;
	vector<int> unvisitedCities = { 0,1,2,3,4,5 };

	while (!DepthFirstTravelsalStack.empty())
	{
		//get new from stack
		SearchNode current = DepthFirstTravelsalStack.top();
		DepthFirstTravelsalStack.pop();		

		//check whether we make a return to some previous node in naive in depth search and make sure visited nodes are handled properly
		while (visited.size() >= 1 && current.parentCityID != visited[visited.size() - 1].cityID)
		{
			int erased = visited[visited.size()-1].cityID;
			visited.pop_back();
			unvisitedCities.push_back(erased);
		}
		
		//put new node on visited list
		visited.push_back(current);
		for (std::vector<int>::iterator i = unvisitedCities.begin(); i < unvisitedCities.end(); i++)
		{
			if (*i == current.cityID)
			{

				unvisitedCities.erase(i);
				break;
			}
		}

		//add childs of current onto stack so we can visit them later.
		for (int i = NumberOfCities - 1; i >= 0; i--)
		{
			if (!DoesElementExist(visited, i) && actions[current.cityID][i] < INT_MAX)
			{
				DepthFirstTravelsalStack.push(SearchNode(i, current.cityID));
			}
		}

		//this is just a debug.
		if (true)//visited.size() == NumberOfCities)
		{
			int length = 0;
			for (size_t i = 0; i < visited.size(); i++)
			{
				cout << cities[visited[i].cityID] << " -> ";
				length += actions[visited[i].parentCityID][visited[i].cityID];
			}
			cout <<"Warszawa\t\t Length = " << length + actions[visited[visited.size() - 1].cityID][start];
			cout << "\nUNVISITED : ";
			for (size_t i = 0; i < unvisitedCities.size(); i++)
			{
				cout << cities[unvisitedCities[i]] << ", ";
			}
			cout << endl;
		}
	
		cout << "H1 = " << h1(visited, unvisitedCities, actions, start) << endl;

		
	}
	return 0;
}
