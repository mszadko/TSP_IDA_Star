#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <stack>

#include <ctime>
#include <fstream>

#include "InputParser.h"
#include "SearchNode.h"


using namespace std;
vector<string> cities;
size_t NumberOfCities;
bool IsLoggingEnabled = true;
bool ShouldUseMSTHeuristic = false;


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

int CalculatePathCost(const vector<SearchNode>&visited, const vector<vector<int>>& actions,bool forceLogging=false)
{
	int g = 0;
	for (size_t i = 1; i < visited.size(); i++)
	{
		int currentEdgeCost = actions[visited[i].parentCityID][visited[i].cityID];
		g += currentEdgeCost;
		if (IsLoggingEnabled||forceLogging)
			cout << cities[visited[i-1].cityID] << "---" << currentEdgeCost << "-->";
	}
	if (IsLoggingEnabled||forceLogging)
	{
		cout << cities[visited[visited.size() - 1].cityID];
		cout << endl;
	}		
	return g;
}

vector<SearchNode> IDAStarSolver(const vector<vector<int>>& actions,int CalculateHeuristic(vector<SearchNode> visited, vector<int> unvisited, const vector<vector<int>>& actions, int startNode))
{
	//data needed for IDA* algorithm
	bool AlreadyCalculatedFBoundZero = false;
	stack<SearchNode> DepthFirstTravelsalStack;
	const int start = 0;
	int fBound = 0;
	int fBoundNextCandidate = INT_MAX;
	vector<SearchNode> visited;
	vector<int> unvisitedCities;

	while (visited.size() != NumberOfCities)
	{
		//statistics
		size_t depth = 0;
		size_t numberOfNodes = 0;

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
			numberOfNodes++;
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
			if (visited.size() > depth)
			{
				depth = visited.size();
			}
			if (visited.size()==NumberOfCities)
			{
				break;//we got a solution;
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
				cout << "H = " << h << "\tG = " << g << " \tG+H = " << f << endl;
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
			cout << "\n\n\nEnd of iteration - Next fbound = " << fBound << "\tnumber of searched nodes = " << numberOfNodes << "\tTree depth reached = " << depth << endl;
	}

	int lastCity = visited[visited.size() - 1].cityID;
	visited.push_back(SearchNode(start, lastCity));
	return visited;
}

int main()
{
	cout << INT_MAX << endl;
	cout << "----IDA* TSP Solver---- Marcin Szadkowski 301960 Projekt S3 C++\n\n";
	bool shouldExit = false;
	char key;
	string currentOption;
	string defaultSmallDataPath = "Data/zwykladusmall.txt";
	string defaultMediumDataPath = "Data/dj38parsed.txt";
	string userDefinedDataPath;
	string selectedDataPath = defaultSmallDataPath;

	vector<vector<int>> actions;
	std::vector<SearchNode> soultion;
	
	ofstream results;
	results.open("results.txt");

	clock_t begin_time;
	ShouldUseMSTHeuristic = true;
	IsLoggingEnabled = false;
	if (InputParser::ParseData("Data/zwykladusmall.txt", actions, cities, &NumberOfCities))
	{
		begin_time = clock();
		soultion = IDAStarSolver(actions, ShouldUseMSTHeuristic ? CalculateMinimalSpaningTreeHeuristics : CalculateShortestExitsHeuristic);
		results << "Found solution of first problem in " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
		results << "Found solution! Cycle length = " << CalculatePathCost(soultion, actions, true) << endl << endl;
		int g = 0;
		for (size_t i = 1; i < soultion.size(); i++)
		{
			int currentEdgeCost = actions[soultion[i].parentCityID][soultion[i].cityID];
			g += currentEdgeCost;
			results << cities[soultion[i - 1].cityID] << "---" << currentEdgeCost << "-->";
		}

		results << cities[soultion[soultion.size() - 1].cityID];
		results << endl;
	}
	if (InputParser::ParseData("Data/dj38parsed.txt", actions, cities, &NumberOfCities))
	{
		begin_time = clock();
		soultion = IDAStarSolver(actions, ShouldUseMSTHeuristic ? CalculateMinimalSpaningTreeHeuristics : CalculateShortestExitsHeuristic);
		results << "Found solution of second problem in " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
		results << "Found solution! Cycle length = " << CalculatePathCost(soultion, actions, true) << endl << endl;
		int g = 0;
		for (size_t i = 1; i < soultion.size(); i++)
		{
			int currentEdgeCost = actions[soultion[i].parentCityID][soultion[i].cityID];
			g += currentEdgeCost;
			results << cities[soultion[i - 1].cityID] << "---" << currentEdgeCost << "-->";
		}

		results << cities[soultion[soultion.size() - 1].cityID];
		results << endl;
	}
	if (InputParser::ParseData("qa194parsed.txt", actions, cities, &NumberOfCities))
	{
		begin_time = clock();
		soultion = IDAStarSolver(actions, ShouldUseMSTHeuristic ? CalculateMinimalSpaningTreeHeuristics : CalculateShortestExitsHeuristic);
		results << "Found solution of first problem in " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
		results << "Found solution! Cycle length = " << CalculatePathCost(soultion, actions, true) << endl << endl;
		int g = 0;
		for (size_t i = 1; i < soultion.size(); i++)
		{
			int currentEdgeCost = actions[soultion[i].parentCityID][soultion[i].cityID];
			g += currentEdgeCost;
			results << cities[soultion[i - 1].cityID] << "---" << currentEdgeCost << "-->";
		}

		results << cities[soultion[soultion.size() - 1].cityID];
		results << endl;
	}

	results.close();



	/*
	cout << "Please select option by pressing proper key. Confirm selection by pressing enter:\n\n";
	while (!shouldExit)
	{
		currentOption = IsLoggingEnabled ? "On" : "Off";
		cout << "Press 1 for toggle statistics on/off \t\t\t\t\t(Statistics = " << currentOption << ")" << endl;
		currentOption = ShouldUseMSTHeuristic ? "MinimalSpanningTree" : "ShortestExits";
		cout << "Press 2 for toggle heuristics MinimalSpanningTree/ShortestExits \t(Heuristics =" << currentOption << ")" << endl;
		cout << "Press 3 to select small TSP data. \t\t\t\t\t(Path to data file = " << selectedDataPath << ")" << endl;
		cout << "Press 4 to select medium TSP data.\n";
		cout << "Press 5 to enter own TSP data file.\n";
		cout << "Press 6 to run computation.\n";
		cout << "Press any other key to exit\n";
		cin >> key;
		switch (key)
		{
		case '1':
			IsLoggingEnabled = !IsLoggingEnabled;
			break;
		case '2':
			ShouldUseMSTHeuristic = !ShouldUseMSTHeuristic;
			break;
		case '3':
			selectedDataPath = defaultSmallDataPath;
			break;
		case '4':
			selectedDataPath = defaultMediumDataPath;
			break;
		case '5':
			cin.get();
			cout << "Please enter path to .txt file with your TSP data -\n";
			getline(cin, userDefinedDataPath);
			selectedDataPath = userDefinedDataPath;
			break;
		case '6':
			if (InputParser::ParseData(selectedDataPath, actions, cities, &NumberOfCities))
			{
				soultion = IDAStarSolver(actions, ShouldUseMSTHeuristic ? CalculateMinimalSpaningTreeHeuristics : CalculateShortestExitsHeuristic);
				cout << "Found solution! Cycle length = " << CalculatePathCost(soultion, actions, true) << endl << endl;
			}
			break;
		default:
			shouldExit = true;
			break;
		}
	}*/
	return 0;
}
