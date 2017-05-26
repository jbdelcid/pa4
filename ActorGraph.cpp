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
 * Param: const char * in_filename - file of the list of actors and movies
 *				bool use_weighted_edges - if there are weighted edges
 * Return: bool if it was loaded successfully
 * Loads the file and creates the graph
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges){
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;

  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline( infile, s )) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab character
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    // check for name in hashmap of actors
    unordered_map<string, ActorNode*>::iterator finding = 
      actor.find(actor_name);

    // if not, then create node and call addToGraph
    if( finding == actor.end() ) {
      ActorNode *newNode = new ActorNode(actor_name);
      actor.insert(make_pair((string)actor_name, 
            (ActorNode*)newNode));
      finding = actor.find(actor_name);
    } 

    // else, update node
    string movie = movie_title + '\t' + record[2];

    // find weight
    int weight = 1;
    if( use_weighted_edges ) {
      weight = 1 + (2015 - movie_year);
    }

    ((*finding).second->movies).insert(make_pair((string)movie, (int)weight));

    //add to info hashmap
    unordered_map<string, vector<ActorNode*>>::iterator
      addInfo = info.find(movie);

    //movie not there
    if( addInfo == info.end() ) {
      vector<ActorNode*> listOfActors;
      info.insert(make_pair((string)movie, 
            (vector<ActorNode*>)listOfActors));
      addInfo = info.find(movie);
    }

    ActorNode* temp = (*finding).second;
    //add actor to that movie's vector
    ((*addInfo).second).push_back(temp);	

  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }

  infile.close();

  return true;
}

/**
 * Param: in_filename - name of input file
 * Return: boolean - whether load was successful or not
 * Load in info to populate ActorNodes and actor hashmap
 */
bool ActorGraph::loadFromFile(const char* in_filename) {

  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;
  oldestYear = INT_MAX;

  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline( infile, s )) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab character
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    //check for oldest year
    if( movie_year < oldestYear ) {
      oldestYear = movie_year;
    }

    // check for name in hashmap of actors
    unordered_map<string, ActorNode*>::iterator finding = 
      actor.find(actor_name);

    // if not, then create node and call addToGraph
    if( finding == actor.end() ) {
      ActorNode *newNode = new ActorNode(actor_name);
      actor.insert(make_pair((string)actor_name, 
            (ActorNode*)newNode));
      finding = actor.find(actor_name);
    } 

    // else, update node
    string movie = movie_title + '\t' + record[2];

    // find weight
    int year = movie_year;

    ((*finding).second->movies).insert(make_pair((string)movie, (int)year));
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }

  infile.close();

  return true;
}

/*
 * Param: actor - the hashmap of all actors in the graph
 *				string startActor - the actor whose node we are starting at
 *				string endActor - the actor whose node we are ending at
 * Return: stack - the path names of the actors/movies from start to end
 * Use BFS to search our graph for the shortest path from the first to last
 *		actor.
 */
stack<string> ActorGraph::findPath( string startActor, string endActor ) {
  stack<string> returnStack;

  unordered_map<string, ActorNode*>::iterator findActor = 
    actor.find(startActor);

  unordered_map<string, ActorNode*>::iterator endCheck = 
    actor.find(endActor);

  //check to see if actors are in graph
  if( findActor == actor.end() || endCheck == actor.end() || 
      startActor == endActor ) {

    if( findActor == actor.end() ) {
      cerr << "Failed to locate node '" << startActor << "'" << endl;
    }
    else if ( endCheck == actor.end() ) {
      cerr << "Failed to locate node '" << endActor << "'" << endl;
    }
    return returnStack;
  }


  //use Dijkstra's Algorithm 

  ActorNode* save = nullptr;

  priority_queue<ActorNode*, vector<ActorNode*>, ActorNodeComp> nodePQ;
  ActorNode* start = (*findActor).second;
  start->distance = 0;
  nodePQ.push(start);

  //search for neighbors
  while( !nodePQ.empty() ) {
    ActorNode* curr = nodePQ.top();			
    nodePQ.pop();

    if( !curr->checked ) {
      curr->checked = true;
    }

    unordered_map<string, int>::iterator movieSearch = curr->movies.begin();
    for( ;movieSearch != curr->movies.end(); movieSearch++ ) {
      unordered_map<string, vector<ActorNode*>>::iterator neighborSearch = 
        info.find((*movieSearch).first);
      vector<ActorNode*> addActor = (*neighborSearch).second;

      for( int i = 0; i < addActor.size(); i++ ) {
        ActorNode* next = addActor[i];
        int temp = curr->distance + next->movies.at((*movieSearch).first);

        //new connection made in graph
        if( temp < next->distance && !next->checked ) {
          next->previousMovie = (*movieSearch).first;
          next->previous = curr;
          next->distance = temp;
          nodePQ.push(next);
        }

        if( next -> name == endActor ) {
          save = next;
          break;
        }
      }
    }
  }

  //didn't find end actor
  if( save == nullptr || save->name != endActor ) {
    unordered_map<string, ActorNode*>::iterator reset = actor.begin();
    for( ; reset != actor.end(); reset++ ) {
      (*reset).second->distance = INT_MAX;	
      (*reset).second->previous = nullptr;	
      (*reset).second->previousMovie = "";
      (*reset).second->checked = false;	
    }

    return returnStack;
  }

  //add pathway to stack
  while( save->previous ) {
    //check if single connection
    string add;

    //find movie and year parts of string
    std::size_t split = save->previousMovie.find("\t");

    if( save->previous->name == startActor ) {
      add = "--[" + save->previousMovie.substr(0, split) + "#@" + 
        save->previousMovie.substr(split+1, save->previousMovie.size())
        + "]-->";
      returnStack.push(add);
      break;
    }

    else {
      add = '(' + save->previous->name + ")--[" + 
        save->previousMovie.substr(0, split) + "#@" + 
        save->previousMovie.substr(split+1, save->previousMovie.size())
        + "]-->";
      returnStack.push(add);
    }	
    //go back one node and reset node 
    save = save->previous;
  }

  //Reset graph
  unordered_map<string, ActorNode*>::iterator reset = actor.begin();
  for( ; reset != actor.end(); reset++ ) {
    (*reset).second->distance = INT_MAX;	
    (*reset).second->previous = nullptr;	
    (*reset).second->previousMovie = "";
    (*reset).second->checked = false;	
  }
  return returnStack;
}

/**
 * BFSCONNECT
 * Param: vector<string> startActor - vector of actors we start search with
 *				vector<string> endActor - vector of actors we are connecting to
 * Return: vector<int> - year they were first connected for each pair
 * Finds first connection between the two actors using BFS
 */
vector<int> ActorGraph::BFSConnect( vector<string> startActor, vector<string> endActor ) {

  vector<int> returnYears ( startActor.size(), 9999 );


  //BFS to find earliest connection
  int currentYear = oldestYear;
  //push current to queue


  while( currentYear != 2016 ) {
    //for each actor in actor hashmap
    unordered_map<string, ActorNode*>::iterator actorIT = actor.begin();
    for( ; actorIT != actor.end(); actorIT++ ) {
      //for each movie in hashmap
      unordered_map<string, int>::iterator nodeIT = (*actorIT).second->movies.begin();
      for( ; nodeIT != (*actorIT).second->movies.end(); nodeIT++ ) {
        //if movie == currentYear
        if( (*nodeIT).second != currentYear ) 
          continue;
        //find in info hashmap
        unordered_map<string, vector<ActorNode*>>::iterator infoIT = info.find((*nodeIT).first);
        //if no
        if( infoIT == info.end() ) {
          //add movie to info
          vector<ActorNode*> addActors;
          info.insert(make_pair((string)(*nodeIT).first, (vector<ActorNode*>)addActors)); 
          infoIT = info.find((*nodeIT).first);
        }
        //add actor to info vector
        (*infoIT).second.push_back((*actorIT).second);

      }
    }

    //search whole tree of current year or until end actor is found
    for( int i = 0; i < startActor.size(); i++ ) {
      if( returnYears[i] != 9999 ) 
        continue;

      queue<ActorNode*> toSearch;
      unordered_map<string, ActorNode*>::iterator findActor = actor.find(startActor[i]);
      ActorNode* start = (*findActor).second;
      toSearch.push(start);
      vector<ActorNode*> clearMe;

      while( !toSearch.empty() ) {
        //look at neighbors of current
        ActorNode* next = toSearch.front();
        next->checked = true;
        clearMe.push_back(next);
        toSearch.pop();
        auto it = next->movies.begin();
        for( ; it != next->movies.end(); it++ ) {
          //	if( returnYears[i] != 9999 ) 
          //	break;
          auto it2 = info.find((*it).first);
          if( it2 == info.end() ) {
            continue;
          }

          auto it3 = (*it2).second.begin();
          for( ; it3 != (*it2).second.end(); it3++ ) {

            //	if( returnYears[i] != 9999 ) 
            //	break;
            if( (*it3)->name == endActor[i] ) {
              returnYears[i] = currentYear;
              break;
            }
            if( !(*it3)->checked ) {
              (*it3)->checked = true;
              toSearch.push((*it3));
            }
          }
        }
      }
      vector<ActorNode*>::iterator clear = clearMe.begin();
      for( ; clear != clearMe.end(); clear++ ) {
        (*clear)->checked = false;
      }
    }
    currentYear++;
  }

  //reset graph
  info.clear();
  unordered_map<string, ActorNode*>::iterator clear = actor.begin();
  for( ; clear != actor.end(); clear++ ) {
    (*clear).second->checked = false;
  }
  return returnYears;
}

/**
 * UFINDCONNECT
 * Param: vector<string> startActor - vector of actors we start search with
 *				vector<string> endActor - vector of actors we are connecting to
 * Return: vector<int> - vector of years they were first connected
 * Finds first connection between the two actors using BFS
 */
vector<int> ActorGraph::UFindConnect( vector<string> startActor, vector<string> endActor ) {
  vector<int> returnYears (startActor.size(), 9999);


  int currentYear = oldestYear;
  Union unionSet;
  //for each year
  while( currentYear != 2016 ) {
    //populate info like in BFS
    unordered_map<string, ActorNode*>::iterator actorIT = actor.begin();
    //create hashmap of all nodes for disjoint set
    for( ; actorIT != actor.end(); actorIT++ ) {
      //for each movie in hashmap
      //set all nodes' previous pointers to end()
      //all weights should be 1
      unordered_map<string, int>::iterator nodeIT = (*actorIT).second->movies.begin();
      for( ; nodeIT != (*actorIT).second->movies.end(); nodeIT++ ) {
        if( (*nodeIT).second != currentYear ) 
          continue;
        //find in info hashmap
        unordered_map<string, vector<ActorNode*>>::iterator infoIT = info.find((*nodeIT).first);
        //if no
        if( infoIT == info.end() ) {
          //add movie to info
          vector<ActorNode*> addActors;
          info.insert(make_pair((string)(*nodeIT).first, (vector<ActorNode*>)addActors)); 
          infoIT = info.find((*nodeIT).first);
        }

        //add actor to info vector
        (*infoIT).second.push_back((*actorIT).second);
        //union all info.second nodes into one set

        int size = (*infoIT).second.size();
        if( size == 1 ) {
          ActorNode* root = (*actorIT).second; 
          root->checked = true;
          root->previous = root;
          continue;
        }
        ActorNode* temp = (*infoIT).second.at(size-2);
        unionSet.unionOp( temp, (*actorIT).second, actor );
      }
    }

    /*find -- call on each pair in input vector*/
    for( int i = 0; i < startActor.size(); i++ ) {
      if( returnYears[i] != 9999 ) 
        continue;
      ActorNode* start = unionSet.unionFind( actor.at(startActor[i]), actor );
      ActorNode* end = unionSet.unionFind( actor.at(endActor[i]), actor );


      //if equal, cler prev & return currentYear
      if( start->name == end->name ) {
        returnYears[i] = currentYear;
      }
    }
    //increment year
    currentYear++;
  }

  //if nothing
  info.clear();
  unordered_map<string, ActorNode*>::iterator clearing = actor.begin();
  for( ; clearing != actor.end(); clearing++ ) {
    (*clearing).second->checked = false;
    (*clearing).second->previous = nullptr;
    (*clearing).second->weight = 1;
  }
  return returnYears;
}

/** 
 * Param: None.
 * Return: None.
 *	Deallocated memory associated with the graph.
 */

ActorGraph::~ActorGraph() {
  unordered_map<string, ActorNode*>::iterator startN = actor.begin();
  unordered_map<string, vector<ActorNode*>>::iterator startV = info.begin();

  //delete nodes in actor
  for( ; startN != actor.end(); startN++ ) {
    delete startN->second;
  }

}

