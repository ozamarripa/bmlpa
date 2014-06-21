
#ifndef BMLPA_UNWEIGHTED_H
#define BMLPA_UNWEIGHTED_H


#include "graph_unweighted.h"

using namespace std;

class Algorithm{
public:
	Algorithm(Graph *g);

public:
	void roughcores(map<unsigned int,set<unsigned int> > &roughcore);

	void initbmlpa();
	void propagate_bmlpa(unsigned int x);
	void propagate_bmlpa_2(unsigned int x);
	void propagate_bmlpa_2_vector(unsigned int x,vector<bool>&  ids_vector);
	
	void propagate_bmlpa_parallel(vector<bool>&  ids_vector,unsigned int x);

	void propagate_bmlpa_parallel();
	void normalize(unsigned int x);
	void idl(vector< map<unsigned int, double> >& l, vector<unsigned int>& ids);
	void idl_vector(vector< map<unsigned int, double> >& l, vector<bool>& ids);
	void idl_set(vector< map<unsigned int, double> >& l, vector<unsigned int>& ids);
	void idx(map<unsigned int, double>& lx, vector<unsigned int>& ids);
	void idx_vector(map<unsigned int, double>& lx, vector<bool>& ids);
	void idx_set(map<unsigned int, double>& lx, vector<unsigned int>& ids);
	void count();
	void count_parallel();
	void mc(map<unsigned int, int>& newmin);

	void execute_bmlpa();
	void execute_bmlpa_parallel();

	void removeSubs();

	void splitDisconnectComs();
	void saveResult(string , string);

	void setp(double p);
	void saveTime(string ,string ,char* );



private:
	map< unsigned int, vector<unsigned int> > coms;
	map< unsigned int, vector<unsigned int> > sub;
	Graph *g;
	vector< map<unsigned int, double> > source;
	vector< map<unsigned int, double> > dest;
	map<unsigned int, int> counts;
	map<unsigned int, int> min;
    set<unsigned int> idnew_set;
    set<unsigned int> idold_set;
   
	double p;
	int k;
	map<unsigned int,set<unsigned int> > roughcore;
	

};
#endif //BMLPA_UNWEIGHTED_H
