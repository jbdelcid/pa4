
#ifndef CLASS4_H
#define CLASS4_H

#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include "class2.h"

using namespace std;

class class2;

class class4 {

public:
  class4(void) {}

	~class4(void);

	unordered_map<int, class2*> friendsList;

	/**
	 * Param: in_filename - input filename
	 * Return: boolean saying whether load was successful or not 
	 * Load to the graph
	 */
	bool loadFromFile( const char* in_filename );

	/**
   * Param: start - start friend
   *				end - end friend
   * Return: vector<int> - consists of total, before, and after mutual friends
   * counts
   * Takes in start and end and finds the mutual friends
   */
	int finding( string start, string end );

	

};


#endif // EXTENSIONGRAPH_H

