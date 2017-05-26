#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <climits>
#include <queue>
#include <unordered_map>
#include "ActorGraph.h"
#include "ActorNode.h"
#include "Union.h"

using namespace std;

/**
 * Param: first - the first actor node to union
 *				second - the second actor node to union
 * Return: none.
 * Union the first and second nodes
 */

void Union::unionOp( ActorNode* first, ActorNode* second, unordered_map<string, ActorNode*> & actor  ) {

  first->checked = true;
  second->checked = true;

  ActorNode* firstRoot = unionFind( first, actor );
  ActorNode* secondRoot = unionFind( second, actor );
  if( firstRoot == secondRoot )
    return;

  //		//if more than one movie matches( a previous is already set)
  //		if( first->previous != nullptr ) {
  //merge all actors in those movies
  if( firstRoot->weight >= secondRoot->weight ) {
    secondRoot->previous = firstRoot;
    firstRoot->weight += secondRoot->weight;
    return;
  }

  else {
    firstRoot->previous = secondRoot;
    secondRoot->weight += firstRoot->weight;
    return;
  }

}


/**
 * Param: searching - the node being searched for in the set
 * Return: ActorNode* - the sentinel node of searching
 * Find the node and connect all nodes in path to root
 */

ActorNode* Union::unionFind( ActorNode* searching, unordered_map<string, ActorNode*> & actor ) {
  stack<ActorNode*> compress;

  //cerr << "HELLO" << endl;
  //go to startActor's place in hashmap	
  ActorNode* start = actor.at( searching->name );

  if( !start->previous ) {
    start->previous = start;
    return start;
  }

  int count = 0;
  //trace up previous pointers and add nodes to start
  while( start->previous != start ) {
    compress.push(start);
    start = start->previous;
  }		

  //store root
  ActorNode* sentinel = start;
  //pop each node off stack
  while( !compress.empty() ) {
    if( count != 0 ) 
      //cerr << "Find count is : " << count << endl;
      count++;
    //reassign each previous to point to root
    ActorNode* topping = compress.top();
    topping->previous = start;
    compress.pop();
  }
  return sentinel;

}
