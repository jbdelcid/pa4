#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>


using namespace std;

class ActorNode;

/**
 * Comparator for priority queue
 */
class ActorNodeComp {

public:

	bool operator()(ActorNode*& lhs, ActorNode*& rhs) const {
		return lhs < rhs;
	}
};

class ActorGraph {

protected:  
	//vector of our actor nodes
	unordered_map<string, vector<ActorNode*>> info;
	unordered_map<string, ActorNode*> actor;
	unordered_map<string, vector<string>> connections;

public:
  ActorGraph(void) {}

	~ActorGraph(void);

	int oldestYear;

  /**
   * Load the graph from a tab-delimited file of actor->movie relationships.
   *
   * in_filename - input filename
   * use_weighted_edges - if true, compute edge weights as 1 + 
   *		(2015 - movie_year), otherwise all edge weights will be 1
   *
   * return true if file was loaded sucessfully, false otherwise
   */
  bool loadFromFile(const char* in_filename, bool use_weighted_edges);

	/**
	 * Param: in_filename - input filename
	 * Return: boolean saying whether load was successful or not
	 * Load the actors into nodes and populates graph's actor hashmap
	 */
	bool loadFromFile(const char* in_filename);
	
	/**
	 * Param: string startActor - starting actor
	 *				string endActor - ending actor 
	 * Return: stack<string> - the path names of the actors/movies from start to end
	 * Use BFS to search our graph for the shortest path from the first actor
	 * to the second
	 */
	stack<string> findPath( string startActor, string endActor );

	/**
	 * BFS implementation for actorconnections
	 * Param: vector<string> startActor - vector of actors we are starting at
	 *				vector<string> endActor - vector of actors we are connecting to
	 * Return: vector<int> - the year the two actors were connected for each pair
	 *
	 */
	vector<int> BFSConnect( vector<string> startActor, vector<string> endActor );
	

	/**
	 * ufind implementation for actorconnections
	 * Param: vector<string> startActor - vector of actors we are starting at
	 *				vector<string> endActor - vector of actors we are connecting to
	 * Return: vector<int> - the year the two actors were connected for each pair
	 *
	 */
	vector<int> UFindConnect( vector<string> startActor, vector<string> endActor );

};


#endif // ACTORGRAPH_H
