CC=g++
#CC=mpic++
#CC=tau_cc.sh
#CC=tau_cxx.sh

LOP=-o
#LOPT= -O2
LOPT= -O3
LOPT+=-fopenmp
#LOPT+=-D_GLIBCXX_PARALLEL
#CFLAGS= -g
CFLAGS+=-std=c++0x
CFLAGS+=-funroll-loops
#CFLAGS+=-Wall
#CFLAGS+= -fopenmp


MAIN=./source_code/main_bmlpa_unweighted
GRAPH=./source_code/graph_unweighted
BMLPA=./source_code/bmlpa_unweighted
TAG=bmlpa


$(MAIN).o :
	$(CC) $(CFLAGS) $(LOPT)  $(LOP) $(TAG) $(MAIN).cpp $(GRAPH).cpp $(BMLPA).cpp


