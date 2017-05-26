#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string.h>
#include <stdio.h>
#include "ActorGraph.h"
#include "ActorNode.h"
#include "Union.h"
using namespace std;

/* 
 * Param: argc - number of arguments taken in
 *				argv - array of pointers to the arguments passed in
 * Return: int - whether program was successful or not
 */

int main( int argc, char* argv[] ) {

  bool alg = false;
  ActorGraph * graph = new ActorGraph();

  if( argc != 4 && argc != 5 ) {
    cerr << "Incorrect number of arguments. Try again." << endl;
    return -1;
  }

  //check validity of fourth param
  if( argc == 4 || strcmp( argv[4], "bfs" ) == 0 || strcmp( argv[4], "ufind" ) == 0 ) {
    if( argc == 4 || strcmp( argv[4], "ufind" ) == 0) {
      alg = false;
    }
    else {
      alg = true;
    }
  }

  else {
    cerr << "Invalid fourth argument. Goodbye" << endl;
    return -1;
  }

  bool loadSuccess = graph->loadFromFile( argv[1] );

  //check success of loading
  if( !loadSuccess ) return -1;

  ifstream infile(argv[2]);
  ofstream outfile(argv[3]);

  bool have_header = false;
  vector<string> start_actor;
  vector<string> end_actor;
  vector<int> returnYears;

  //reading file
  while(infile) {

    string s;

    //get the next line
    if( !getline( infile, s )) {
      break;
    }

    //skip header
    if( !have_header ) {	
      have_header = true;
      outfile << "Actor1\tActor2\tYear" << endl;
      continue;
    }

    istringstream ss( s );
    //will hold the start and end actors
    vector <string> record;

    //while there are strings to read in
    while( ss ) {

      string next;

      //get the next string before hitting tab
      if( !getline( ss, next, '\t' )) {
        break;
      }

      record.push_back( next );
    }

    //only read in two names
    if( record.size() != 2 ) {
      continue;
    }

    start_actor.push_back(record[0]);
    end_actor.push_back(record[1]);

  }
  //send information to output file
  if( alg ) {
    returnYears = graph->BFSConnect( start_actor, end_actor );

  }
  else {
    returnYears = graph->UFindConnect( start_actor, end_actor ); 
  }

  for( int i = 0; i < returnYears.size(); i++ ) {
    outfile << start_actor[i] << "\t" << end_actor[i] << "\t" << returnYears[i]
      << endl;
  }

  infile.close();
  delete graph;

  return 0;
}	
