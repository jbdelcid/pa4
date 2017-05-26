
#ifndef UNION_H
#define UNION_H

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class ActorNode;

class ActorGraph;

class Union {

  public:

    /* constructor */
    Union() {};
    /**
     * Param: first - the first actor node to union
     *				second - the second actor node to union
     * Return: none.
     * Union the first and second nodes
     */
    void unionOp( ActorNode* first, ActorNode* second, unordered_map<string, ActorNode*> & actor  );


    /**
     * Param: searching - the node being searched for in the set
     * Return: ActorNode* - the sentinel node of searching
     * Find the node and connect all nodes in path to root
     */
    ActorNode* unionFind( ActorNode* searching, unordered_map<string, ActorNode*> & actor );

};

#endif // UNION_H
