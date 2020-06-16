#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <stack>
#include <sstream>
#include <iterator>

using namespace std;
vector<string> cities;
const int WWA = 0;
const int LDZ = 1;
const int KAT = 2;
const int KRK = 3;
const int LBN = 4;
const int KIE = 5;
int NumberOfCities = 6;

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


int h1(vector<SearchNode> visited, vector<int> unvisited, int **actions, int startNode)
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

int MST(vector<int> unvisited, int **actions)
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

int h2(vector<SearchNode> visited, vector<int> unvisited, int **actions, int startNode)
{
	if (unvisited.size() == 0)
	{
		return actions[visited[visited.size() - 1].cityID][startNode];
	}
	int h2 = MST(unvisited, actions);
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

vector<SearchNode> IDAStarSolver(int **actions)
{

	return vector<SearchNode>();
}


std::vector<int> split(string&s, char delimiter)
{
	vector<int> tokens;
	string token;
	istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter))
	{
		istringstream ss(token);
		int x;
		ss >> x;
		tokens.push_back(x);
	}
	return tokens;
}

void ParseData(string dataFile,int **actions)
{
	ifstream dataStream;
	dataStream.open(dataFile);
	if (!dataStream)
	{
		cout << "Bad file\n";
	}
	string line;
	getline(dataStream, line);
	istringstream ss(line);
	ss >> NumberOfCities;
	cout << NumberOfCities;


	getline(dataStream, line);
	ss = istringstream(line);
	cities.clear();
	string city;
	while (getline(ss, city, ';'))
	{
		cities.push_back(city);
	}

	cout << endl;
	int index = 0;
	while (getline(dataStream, line, ';'))
	{
		istringstream ss(line);
		int x;
		ss >> x;
		actions[index / NumberOfCities][index % NumberOfCities] = x;
		index++;
	}
}

int main()
{
	stack<SearchNode> DepthFirstTravelsalStack;
	int ** actions = new int*[NumberOfCities];
	for (size_t i = 0; i < NumberOfCities; i++)
	{
		actions[i] = new int[NumberOfCities];
	}

	ParseData("Data/zwykladu.txt", actions);

	int start = WWA;
	DepthFirstTravelsalStack.push(SearchNode(start, start));
	vector<SearchNode> visited;
	vector<int> unvisitedCities = { 0,1,2,3,4,5 };

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
		int g = 0;

		for (size_t i = 0; i < visited.size(); i++)
		{
			cout << cities[visited[i].cityID] << " -> ";
			g += actions[visited[i].parentCityID][visited[i].cityID];
		}
		cout << endl;

		int hone = h1(visited, unvisitedCities, actions, start);
		int htwo = h2(visited, unvisitedCities, actions, start);
	
		cout << "H1 = " << hone << " \tG+H1 = " << hone + g << endl;
		cout << "H2 = " << htwo << " \tG+H2 = " << htwo + g << endl;

		//add childs of current onto stack so we can visit them later.
		for (int i = NumberOfCities - 1; i >= 0; i--)
		{
			if (!DoesElementExist(visited, i) && actions[current.cityID][i] < INT_MAX)
			{
				DepthFirstTravelsalStack.push(SearchNode(i, current.cityID));
			}
		}

	}

	//cleaning memory that was allocated for action matrix;
	for (size_t i = 0; i < NumberOfCities; i++)
	{
		delete[] actions[i];
	}
	delete[] actions;

	return 0;
}
