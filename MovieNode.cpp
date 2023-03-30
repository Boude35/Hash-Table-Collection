#include "MovieNode.hpp"

using namespace std;

//prints the value of the node
ostream& operator<<(ostream &os, const MovieNode &node)
{
	os<< *node.value();
	return os;
}



