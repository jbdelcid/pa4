#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string.h>
#include <stdio.h>
#include <unordered_map>
#include <vector>
#include "class4.h"
#include "class2.h"

using namespace std;

/* 
 * Param: argc - number of arguments taken in
 *				argv - array of pointers to the arguments passed in
 * Return: int - whether program was successful or not
 */

int main( int argc, char* argv[] ) {

  if( argc != 4 ) {
    cerr << "Incorrect number of arguments. Try again." << endl;
    return -1;
  }

  //check validity of fourth param
  class4 * graph = new class4();
  bool loadSuccess = graph->loadFromFile( argv[1] );

  //check success of loading
  if( !loadSuccess ) return -1;

  ifstream infile(argv[2]);
  ofstream outfile(argv[3]);

  int mutualCounts = 0;

  //reading file
  while(infile) {

    string s;

    //get the next line
    if( !getline( infile, s )) {
      break;
    }

    istringstream ss( s );
    //will hold the start and end actors
    vector <string> record;

    //while there are strings to read in
    while( ss ) {

      string next;

      //get the next string before hitting tab
      if( !getline( ss, next, ' ' )) {
        break;
      }

      record.push_back( next );
    }

    //only read in two names
    if( record.size() != 2 ) {
      continue;
    }

    string start_friend(record[0]);
    string end_friend(record[1]);

    //send information to output file
    mutualCounts = graph->finding( start_friend, end_friend ); 

    outfile << "# of connections between " << start_friend << " and "
      << end_friend << " are " << mutualCounts << endl;
  }

  infile.close();
  delete graph;

  return 0;

}
