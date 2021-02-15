#ifndef _POOL_H
#define _POOL_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Pool {
public:	
	vector<char> poolPieces;

public:	

	//void addToPool(string toAdd);
	void addToPool(char toAdd);
};

#endif