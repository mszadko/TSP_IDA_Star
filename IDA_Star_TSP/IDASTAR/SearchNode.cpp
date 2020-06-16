#include "SearchNode.h"
SearchNode::SearchNode(int ID, int parentID)
{
	cityID = ID;
	parentCityID = parentID;
}
bool SearchNode::Equals(SearchNode other)
{
	return cityID == other.cityID;
}
bool SearchNode::Equals(int otherID)
{
	return cityID == otherID;
}