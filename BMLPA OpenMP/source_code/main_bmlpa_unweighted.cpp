//************************
//Coded by Zhihao Wu
//Last update: June, 2012
//************************

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <omp.h>

#include "graph_unweighted.h"
#include "bmlpa_unweighted.h"


using namespace std;

char *inputfile = NULL;
char *n2c_outputfile = NULL;
char *c2n_outputfile =NULL;
double para_p = 0.7;


void parse_args(int argc, char **argv)
{
	int i;
	if(argc < 2)
	{
		cerr<<"options: "<<endl;
		cerr<<"giving inputfile using: -i inputfilepath"<<endl;
		cerr<<"giving outputfile in n2c format using: -on2c outputfilepath"<<endl;
		cerr<<"giving outputfile in c2n format using: -oc2n outputfilepath"<<endl;
		cerr<<"giving parameter p using: -p paramerter_p"<<endl;
		exit(0);
	}
	for(i = 1; i<argc; i++)
	{
		string temp = argv[i];
		if(temp == "-i")
		{
			inputfile = argv[i+1];
			i++;
		}else if(temp == "-p")
		{
			para_p = atof(argv[i+1]);
			i++;
		}else if(temp == "-on2c")
		{
			n2c_outputfile = argv[i+1];
			i++;
		}else if(temp == "-oc2n")
		{
			c2n_outputfile = argv[i+1];
			i++;
		}else{
			cerr<<"error parameter!"<<endl;
			cerr<<"options: "<<endl;
			cerr<<"giving inputfile using: -i inputfilepath"<<endl;
			cerr<<"giving outputfile in n2c format using: -on2c outputfilepath"<<endl;
			cerr<<"giving outputfile in c2n format using: -oc2n outputfilepath"<<endl;
			cerr<<"giving parameter p using: -p paramerter_p"<<endl;
			exit(0);
			
		}
	}
}

int main(int argc, char **argv)
{
	 struct timeval timstr;      /* structure to hold elapsed time */
	  struct rusage ru;           /* structure to hold CPU time--system and user */
	  double tic,toc;             /* floating point numbers to calculate elapsed wallclock time */
	  double usrtim;              /* floating point number to record elapsed user CPU time */
	  double systim;              /* floating point number to record elapsed system CPU time */

	//parse parameters
	parse_args(argc, argv);
    
    int nthreads, tid;
    //omp_set_num_threads(8);
	  /* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel private(tid)
	  {
	    /* Obtain thread number */
	    tid = omp_get_thread_num();
	    //printf("Hello, world from thread = %d\n", tid);

	    /* Only master thread does this */
	#pragma omp master
	    {
	      nthreads = omp_get_num_threads();
	      printf("Number of threads = %d\n", nthreads);
	    }
	  }  /* All threads join master thread and disband */

	

    gettimeofday(&timstr,NULL);
  	tic=timstr.tv_sec+(timstr.tv_usec/1000000.0);
	//read the network data
	Graph *g;
	g = new Graph(inputfile, false);

	cout<<"number of nodes: "<<g->links.size()<<endl;
	cout<<"number of edges: "<<g->nb_links<<endl;

	Algorithm *algorithm;
	algorithm = new Algorithm(g);

        //cout<<"before setp"<<endl;
	//set parameter p
	algorithm->setp(para_p);

        //cout<<"before exec"<<endl;
	//execute bmlpa algorithm
	algorithm->execute_bmlpa_parallel();

	gettimeofday(&timstr,NULL);
	  toc=timstr.tv_sec+(timstr.tv_usec/1000000.0);
	  getrusage(RUSAGE_SELF, &ru);
	  timstr=ru.ru_utime;        
	  usrtim=timstr.tv_sec+(timstr.tv_usec/1000000.0);
	  timstr=ru.ru_stime;        
	  systim=timstr.tv_sec+(timstr.tv_usec/1000000.0);
        
	std::string timeFileName;
	if(n2c_outputfile!=NULL)
	{
		algorithm->saveResult(n2c_outputfile, "n2c");
		
	}

	if(c2n_outputfile!=NULL)
	{
		algorithm->saveResult(c2n_outputfile,"c2n");
		
	}
	cout<<"Done."<<endl;

	
    char buff[1024];
    sprintf(buff, "Elapsed time:\t\t\t%.6lf (s)\nElapsed user CPU time:\t\t%.6lf (s)\nElapsed system CPU time:\t%.6lf (s)\n", toc-tic,usrtim,systim);
	std::string time=buff;
	cout<<time<<endl;
    std::string inputfile_s=inputfile;
    inputfile_s="Time_"+inputfile_s;
	algorithm->saveTime(inputfile_s,time,inputfile);

	return 0;
}
