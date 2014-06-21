// File: graph_unweighted.h


#ifndef GRAPH_UNWEIGHTED_H
#define GRAPH_UNWEIGHTED_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <time.h>
#include <sstream>
//#include <io.h>
#include <stack>
//#include <windows.h>
#include <limits.h>
//#include <direct.h>

using namespace std;


class Graph {
 public:
  unsigned int nb_links;
  unsigned int nb_nodes;
  vector<vector<unsigned int> > links;
  //vector<vector<unsigned int> > links_temp;
  map<string,unsigned int> nodeMap;
  map<unsigned int,string> converseNodeMap;
  vector<vector<int> > links_lg; 
  //vector<int> node_degree;
  Graph (const char *filename, bool weight_if);
  
  //void clean();
  //void readGraph(char *filename, bool char_if);
  //void renumber();
  //void display();
  //void output_lineGraph(char *filename); 
};

#endif // GRAPH_UNWEIGHTED_H
