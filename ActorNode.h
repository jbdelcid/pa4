
#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class ActorGraph;

class Union;

class ActorNode {

  public:

    /* Constructor */
    ActorNode(std::string name);

    /* Actor name */
    std::string name;

    /* weight for ufind */
    int weight;

    /* counter of distance */
    int distance;

    /* year tracker for ufind */
    int year;

    /* checked boolean */
    bool checked;

    /* hashmap of movies for actor with weights */
    unordered_map<std::string, int> movies;

    /* pointer to previous node in current search */
    ActorNode * previous;

    /* current movie and year*/
    std::string previousMovie;

    /* overloaded comparison operator for node */
    bool operator<(const ActorNode& other); 

}; 

#endif // ACTORNODE_H
