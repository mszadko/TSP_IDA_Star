#pragma once
class SearchNode
{
public:
	SearchNode(int ID, int parentID);
	
	bool Equals(SearchNode other);
	
	bool Equals(int otherID);
	
	int cityID;
	int parentCityID;
};