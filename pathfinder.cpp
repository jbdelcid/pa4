#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <stdlib.h>
#include "helper.h"
#include "ActorGraph.h"
#include "ActorNode.h"
using namespace std;


/* 
 * Param: argc - number of arguments taken in
 *				argv - array of pointers to the arguments passed in
 * Return: int - whether program was successful or not
 */

int main( int argc, char* argv[] ) {

  bool weighted = false;
  ActorGraph * graph = new ActorGraph();
  Timer time;

  //do checks on all input

  //check if number of args is correct
  if( argc != 5 ) {
    cerr << "Not the right number of arguments. Goodbye." << endl;
    return -1;
  }

  //check weighted boolean
  if( *argv[2] != 'u' && *argv[2] != 'w' ) {
    cerr << "Invalid argument. Try later." << *argv[2] << endl;
    return -1;
  }

  //call on weighted or unweighted?
  if( *argv[2] == 'w' ) {
    weighted = true;
  }

  time.begin_timer();

  bool loadSuccess = graph->loadFromFile( argv[1], weighted );

  //check success of loading
  if( !loadSuccess ) return -1;

  ifstream infile(argv[3]);
  ofstream outfile(argv[4]);

  bool have_header = false;

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
      outfile << "(actor)--[movie#@year]-->(actor)--..." << endl;
      continue;
    }

    istringstream ss( s );
    //will hold the start and end actors
    vector <string> record;

    //while there are strings to read in
    while( ss ) {

      string next;

      //get the next string before hitting a tab character
      if( !getline( ss, next, '\t' )) {
        break;
      }

      record.push_back( next );
    }

    //only read in two names
    if( record.size() != 2 ) {
      continue;
    }

    string start_actor(record[0]);
    string end_actor(record[1]);

    //send information to output file
    stack<string> pathway = 
      graph->findPath( start_actor, end_actor );
    if( pathway.empty() ) {
      continue;	
    } 

    outfile << "(" << start_actor << ")";

    //pop off pathway from stack
    while( !pathway.empty() ) {
      outfile << pathway.top();
      pathway.pop();
    }

    //add last actor to line
    outfile << "(" << end_actor << ")\n";
  }

  if( !infile.eof() ) {
    cerr << "Failed to read " << argv[3] << "!\n";
    return -1;
  }

  long long timing = time.end_timer();
  cout << "Time taken: " << timing << endl;
  infile.close();	
  delete graph;	

  return 0;

}
