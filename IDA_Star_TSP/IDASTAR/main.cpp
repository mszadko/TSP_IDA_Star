#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <stack>

#include "InputParser.h"
#include "SearchNode.h"


using namespace std;
vector<string> cities;
size_t NumberOfCities;
bool IsLoggingEnabled = true;


bool DoesVectorContainElement(std::vector<SearchNode> vector, int element)
{
	for (std::vector<SearchNode>::iterator i = vector.begin(); i < vector.end(); i++)
	{
		if ((*i).cityID==element)
		{
			return true;
		}
	}
	return false;
}

int CalculateShortestExitsHeuristic(vector<SearchNode> visited, vector<int> unvisited, const vector<vector<int>>& actions, int startNode)
{
	if (unvisited.size() == 0)
	{
		return actions[visited[visited.size() - 1].cityID][startNode];
	}
	int h1 = 0;
	
	int shortestConnection = INT_MAX;
	int shortestWayOut = INT_MAX;

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
				}
			}
		}
	}

	h1 = shortestConnection;

	//cities that will be added to heuristic
	int f = 0, t = 0;

	for (size_t i = 0; i < unvisited.size(); i++)
	{
		shortestWayOut = INT_MAX;
		int from = unvisited[i];
		f = 0; t = 0;

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

			}

		}
		if (actions[from][startNode] < shortestWayOut)
		{
			shortestWayOut = actions[from][startNode];
			f = from;
			t = startNode;
		}

		if (f != t)
		{
			h1 += shortestWayOut;
		}
	}
	return h1;
}

int CalculateMinimalSpanningTreeLength(vector<int> unvisited, const vector<vector<int>>& actions)
{
	int mstLength = 0;
	vector<int> visited;
	vector<SearchNode> edges;
	if (unvisited.size()==0)
	{
		return 0;
	}
	int current = unvisited[unvisited.size()-1];
	unvisited.pop_back();
	visited.push_back(current);
	
	while (unvisited.size()>0)
	{
		int f = 0, t = 0;
		int shortestConnection = INT_MAX;

		//look for shortest way to connect visited cities with unvisited ones
		for (size_t i = 0; i < visited.size(); i++)
		{
			int from = visited[i];
			for (size_t j = 0; j < unvisited.size(); j++)
			{
				int to = unvisited[j];
				if (from != to && actions[from][to] < INT_MAX)
				{
					if (actions[from][to] < shortestConnection)
					{
						shortestConnection = actions[from][to];
						f = from;
						t = to;
					}
				}
			}
		}
		edges.push_back(SearchNode(f, t));
		mstLength += shortestConnection;
		visited.push_back(t);
		for (std::vector<int>::iterator i = unvisited.begin(); i < unvisited.end(); i++)
		{
			if (*i == t)
			{
				unvisited.erase(i);
				break;
			}
		}
	}
	return mstLength;
}

int CalculateMinimalSpaningTreeHeuristics(vector<SearchNode> visited, vector<int> unvisited, const vector<vector<int>>& actions, int startNode)
{
	if (unvisited.size() == 0)
	{
		return actions[visited[visited.size() - 1].cityID][startNode];
	}
	int h2 = CalculateMinimalSpanningTreeLength(unvisited, actions);
	int smallestEntryToStartNode = INT_MAX;
	int smallestExitFromLastlyVisied = INT_MAX;
	int lastlyVisited = visited[visited.size() - 1].cityID;
	for (size_t i = 0; i < unvisited.size(); i++)
	{
		int other = unvisited[i];
		if (other != startNode && actions[other][startNode] < smallestEntryToStartNode)
		{
			smallestEntryToStartNode = actions[other][startNode];
		}
		if (other != lastlyVisited && actions[lastlyVisited][other] < smallestExitFromLastlyVisied)
		{
			smallestExitFromLastlyVisied = actions[lastlyVisited][other];
		}
	}

	
	h2 = h2 + smallestEntryToStartNode + smallestExitFromLastlyVisied;
	return h2;
}

int CalculatePathCost(const vector<SearchNode>&visited, const vector<vector<int>>& actions)
{
	int g = 0;
	for (size_t i = 1; i < visited.size(); i++)
	{
		int currentEdgeCost = actions[visited[i].parentCityID][visited[i].cityID];
		g += currentEdgeCost;
		if (IsLoggingEnabled)
			cout << cities[visited[i-1].cityID] << "---" << currentEdgeCost << "-->";
	}
	if (IsLoggingEnabled)
	{
		cout << cities[visited[visited.size() - 1].cityID];
		cout << endl;
	}		
	return g;
}

vector<SearchNode> IDAStarSolver(const vector<vector<int>>& actions,int CalculateHeuristic(vector<SearchNode> visited, vector<int> unvisited, const vector<vector<int>>& actions, int startNode))
{
	bool AlreadyCalculatedFBoundZero = false;
	stack<SearchNode> DepthFirstTravelsalStack;
	const int start = 0;
	int fBound = 0;
	int fBoundNextCandidate = INT_MAX;
	vector<SearchNode> visited;
	vector<int> unvisitedCities;

	while (true)
	{
		DepthFirstTravelsalStack = stack<SearchNode>();
		DepthFirstTravelsalStack.push(SearchNode(start, start));
		visited = vector<SearchNode>();
		unvisitedCities = vector<int>();
		for (size_t i = 0; i < NumberOfCities; i++)
		{
			if (i!=start)
			{
				unvisitedCities.push_back(i);
			}
		}

		while (!DepthFirstTravelsalStack.empty())
		{
			//get new city from stack
			SearchNode current = DepthFirstTravelsalStack.top();
			DepthFirstTravelsalStack.pop();

			//check whether we make a return to some previous node in naive in depth search and make sure visited nodes are handled properly
			while (visited.size() >= 1 && current.parentCityID != visited[visited.size() - 1].cityID)
			{
				int erased = visited[visited.size() - 1].cityID;
				visited.pop_back();
				unvisitedCities.push_back(erased);
			}

			//put new node on visited list
			visited.push_back(current);

			if (visited.size()==NumberOfCities)
			{
				int lastCity = visited[visited.size() - 1].cityID;
				visited.push_back(SearchNode(start, lastCity));
				return visited;
			}

			for (std::vector<int>::iterator i = unvisitedCities.begin(); i < unvisitedCities.end(); i++)
			{
				if (*i == current.cityID)
				{
					unvisitedCities.erase(i);
					break;
				}
			}

			//calculate cost of current node
			int f = 0;
			int g = CalculatePathCost(visited, actions);
			int h = CalculateHeuristic(visited, unvisitedCities, actions, start);
			f = g + h;
			if (!AlreadyCalculatedFBoundZero)
			{
				AlreadyCalculatedFBoundZero = true;
				fBound = f;
				fBoundNextCandidate = INT_MAX;
				if (IsLoggingEnabled)
				{
					cout << "First F bound = " << fBound << endl;
				}
			}
			if (IsLoggingEnabled)
				cout << "H = " << h << "\tG = " << g << " \tG+H1 = " << f << endl;
			if (f > fBound&&f < fBoundNextCandidate)
			{
				fBoundNextCandidate = f;
			}
			if (f <= fBound)
			{
				//add childs of current onto stack so we can visit them later.
				for (int i = NumberOfCities - 1; i >= 0; i--)
				{
					if (!DoesVectorContainElement(visited, i) && actions[current.cityID][i] < INT_MAX)
					{
						DepthFirstTravelsalStack.push(SearchNode(i, current.cityID));
					}
				}
			}
		}
		fBound = fBoundNextCandidate;
		fBoundNextCandidate = INT_MAX;
		if (IsLoggingEnabled)
			cout << "\n\n\nNext fbound = " << fBound << endl;
	}

	int lastCity = visited[visited.size() - 1].cityID;
	visited.push_back(SearchNode(lastCity, start));
	return visited;
}


int main()
{
	vector<vector<int>> actions;

	string s = "Data/zwykladu.txt";

	InputParser::ParseData(s, actions, cities, &NumberOfCities);

	std::vector<SearchNode> foundCycle = IDAStarSolver(actions, CalculateShortestExitsHeuristic);
	
	//std::vector<SearchNode> foundCycle = IDAStarSolver(actions, CalculateMinimalSpaningTreeHeuristics);

	cout << CalculatePathCost(foundCycle, actions);
	
	return 0;
}
