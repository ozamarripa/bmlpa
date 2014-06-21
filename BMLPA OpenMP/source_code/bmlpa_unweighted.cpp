
#include "bmlpa_unweighted.h"
#include <iterator>
#include <set>
#include <array>
using namespace std;

Algorithm::Algorithm(Graph *g)
{
	this->g = g;
	this->p = 0.75;
	this->k = 3;
}


void Algorithm::roughcores(map<unsigned int,set<unsigned int> > &communities)
{
	vector<unsigned int> label_num;
	
	//map<unsigned int,set<unsigned int> > communities;

	vector<pair<unsigned int,unsigned int> > sortDegree;
	
	for(unsigned int i=0; i<g->links.size(); i++)
	{
		sortDegree.push_back(make_pair(g->links[i].size(),i));
		label_num.push_back(1);
	}
	sort(sortDegree.begin(),sortDegree.end());


	map<unsigned int,unsigned int> getAttached;

        //cout<<"before for"<<endl;
        //cout<<"sortDegree.size = "<<sortDegree.size()<<endl;

	for(int i=sortDegree.size()-1; i>=0 && sortDegree[i].first >= k; i--)
	{
           // cout<<"i: "<<i<<endl;

		if(!getAttached[sortDegree[i].second])
		{

                    //cout<<"if"<<endl;

			unsigned int maxDegree=0;
			unsigned int maxDNode;
			for(unsigned int j=0; j<g->links[sortDegree[i].second].size(); j++)
			{
				if( !getAttached[g->links[sortDegree[i].second][j]] && g->links[ g->links[sortDegree[i].second][j] ].size() >=k)
				{
					if(g->links[ g->links[sortDegree[i].second][j] ].size() > maxDegree)
					{
						maxDegree = g->links[ g->links[sortDegree[i].second][j] ].size();
						maxDNode  = g->links[sortDegree[i].second][j];
					}
			 	}
			}

                        //cout<<"111111"<<endl;
			set<unsigned int> lcommunity;

			if(maxDegree!=0)//find free node with max degree
			{
				label_num[maxDNode]++;
				getAttached[sortDegree[i].second]=1;
				getAttached[maxDNode]=1;
				
				lcommunity.insert(maxDNode);
				lcommunity.insert(sortDegree[i].second);
				
				
				map<unsigned int,unsigned int> expandNeiber;
				for(set<unsigned int>::iterator it=lcommunity.begin(); it!=lcommunity.end(); it++)
				{

					for(unsigned int j=0; j<g->links[*it].size(); j++)
					{
						if(g->links[ g->links[*it][j] ].size() >= k)
							expandNeiber[g->links[*it][j]]++;

					}
				}


				unsigned int maxDCandiDegree=0;
				unsigned int maxDCandiNode;
				set< pair<unsigned int,unsigned int> > candiNodes;//<degree£¬node id>
				for(map<unsigned int,unsigned int>::iterator it= expandNeiber.begin(); it!=expandNeiber.end(); it++)
				{
					if(it->second==label_num[maxDNode])
					{
						candiNodes.insert(make_pair(g->links[it->first].size(),it->first));
					}
				}

				unsigned int attachedNodesNum=0;

                                //cout<<"22222222"<<endl;

				while(candiNodes.size()!=0)
				{
					if(!getAttached[ candiNodes.begin()->second ])
					{
						getAttached[candiNodes.begin()->second]=1;
					}else{
	
					}
					label_num[maxDNode]++;
					
					lcommunity.insert(candiNodes.begin()->second);

					unsigned int justInNode=candiNodes.begin()->second;

					candiNodes.erase(candiNodes.begin());

                                       // cout<<"333333"<<endl;
					for(set<pair<unsigned int,unsigned int> >::iterator it=candiNodes.begin(); it!=candiNodes.end();)
					{
						vector<unsigned int>::iterator it1;
						it1=lower_bound(g->links[justInNode].begin(),g->links[justInNode].end(),it->second);
                                                if(it1==g->links[justInNode].end() || (*it1) != it->second)
                                                {
                                                        candiNodes.erase(it++);
                                                }else {
                                                        it++;
                                                }

                                        }

                                }

                                //cout<<"444444"<<endl;
	
				communities[maxDNode] = lcommunity;
				
			}

		}
	}

}







void Algorithm::initbmlpa()
{
	source.clear();
	dest.clear();

	set<unsigned int> unusedL;

	for(unsigned int i = 0; i < this->g->nb_nodes; i++)
	{
		unusedL.insert(i);

		map<unsigned int, double> mp;

		this->source.push_back(mp);
		this->dest.push_back(mp);
	}

        //cout<<"before roughcore"<<endl;
	roughcores(roughcore);
        //cout<<"after roughcore"<<endl;

	//°Ñrough coreµÄ±êÇ©¸øÏàÓ¦µÄ½Úµã
	for(map<unsigned int, set<unsigned int> >::iterator itm = roughcore.begin(); itm != roughcore.end(); itm++)
	{
		unusedL.erase(itm->first);

		for(set<unsigned int>::iterator its = itm->second.begin(); its != itm->second.end(); its++)
		{
			//source[*its].insert(make_pair(itm->first,0));
			source[*its][itm->first]+=1.0;
		}
	}

	//¼ÆËã½ÚµãµÄ±êÇ©Á¥Êô¶È£¬Ã»ÓÐ±êÇ©µÄ½Úµã¸ø±êÇ©
	for(unsigned int i=0; i<source.size(); i++)
	{
		if(source[i].size() == 0)
		{
			source[i].insert(make_pair(*(unusedL.begin()),1.0));
			unusedL.erase(unusedL.begin());
			

		}else{

			double sum = 0;

			for(map<unsigned int, double>::iterator itm = source[i].begin(); itm !=source[i].end(); itm++)
			{
				sum += itm->second;
				//itm->second = (double)1/(double)source[i].size();
			}

			for(map<unsigned int, double>::iterator itm = source[i].begin(); itm !=source[i].end(); itm++)
			{
				//sum+=itm->second;
				itm->second = itm->second/sum;
			}
		}
	}

	this->count();
	this->min = this->counts;

}


void Algorithm::execute_bmlpa_parallel()
{
	min.clear();
	counts.clear();
	source.clear();
	dest.clear();
	coms.clear();
	sub.clear();
	// 1. ³õÊ¼»¯
	const unsigned int num_nodes=this->g->nb_nodes;
       
     this->initbmlpa();



	// 2. ±êÇ©´«²¥
	int count_iter = 0;
	unsigned int i;

	vector<bool> idold_vector (num_nodes, false);
	vector<bool> idnew_vector (num_nodes, false);
	bool iteration=true;
	
	this->idl_vector(source, idnew_vector);
	#pragma omp parallel default(none) shared(idnew_vector,idold_vector,count_iter,iteration) 
	{
	while(iteration)
	{
		#pragma omp single
		{
			count_iter++;
			idold_vector=idnew_vector;
		}

		#pragma omp for
		for(unsigned int x = 0; x < this->g->nb_nodes; x++){
			idnew_vector[x]=false;
		}
		 
		#pragma omp for schedule(static,20)
		for(unsigned int x = 0; x < this->g->nb_nodes; x++){
			this->propagate_bmlpa_parallel(idnew_vector,x);
		}
	
		#pragma omp single
		{
			map<unsigned int, int> newmin;
			this->count();
			if(idold_vector == idnew_vector){
				this->mc(newmin);
				
			}else{
				newmin = this->counts;
				
			}
		
			iteration=false;
			if(newmin != this->min)
			{
				this->min = newmin;
				this->source = this->dest;
				iteration=true;
			}
		}

		
	}
}
	cout<<"number of iterations of label propagation: "<<count_iter<<endl;
	removeSubs();
	splitDisconnectComs();


}




inline void Algorithm::propagate_bmlpa_parallel(vector<bool>& ids_vector,unsigned int x)
{
	
	//1. Vector with neighboors
	vector<unsigned int> nx = g->links[x];
	dest[x].clear();

    double maxb_1 = 0;
	//2. Obtains beloging coefficient for each label of its neighboors
	for(vector<unsigned int>::iterator nxi = nx.begin(); nxi != nx.end(); nxi++)
	{
		unsigned int nxId = *nxi;

		map<unsigned int, double> ncb = source[nxId];
		for(map<unsigned int, double>::iterator ncbi = ncb.begin(); ncbi !=ncb.end(); ncbi++)
		{
			unsigned int c = ncbi->first;

			dest[x][c] += ncbi->second;
			if(dest[x][c]>maxb_1)
				maxb_1=dest[x][c];
		}
	}

	double sum_1 = 0.0;
	//4. Checking if the Belonging value is higher than threshold p to retain the value
	for(map<unsigned int, double>::iterator it = dest[x].begin(); it != dest[x].end(); )
	{
		if(it->second/maxb_1 < this->p)
			dest[x].erase(it++);
		else{
			//this->idnew_set.insert(it->first);
			ids_vector[it->first]=true;
			sum_1+=it->second;
			it++;
		}
	}


	for(map<unsigned int, double>::iterator destxi = dest[x].begin(); destxi != dest[x].end(); destxi++)
		destxi->second = destxi->second / sum_1;

	

}



void Algorithm::normalize(unsigned int x)
{
	double sum = 0.0;
	for(map<unsigned int, double>::iterator destxi = dest[x].begin(); destxi != dest[x].end(); destxi++)
		sum += destxi->second;
	for(map<unsigned int, double>::iterator destxi = dest[x].begin(); destxi != dest[x].end(); destxi++)
		destxi->second = destxi->second / sum;
	/*if(sum_1!=sum)
		cout<<"Difference"<<endl;
	else
		cout<<"Equal"<<endl;*/
}

void Algorithm::removeSubs()
{
	coms.clear();
	sub.clear();
	
	for(unsigned int i=0; i < g->links.size(); i++)
	{
		//for each vertex
		vector<unsigned int> ids;
		idx(dest[i],ids);
		sort(ids.begin(),ids.end());
		for(vector<unsigned int>::iterator it = ids.begin(); it != ids.end(); it++)
		{
			map<unsigned int, vector<unsigned int> >::iterator mit = coms.find(*it);
			map<unsigned int, vector<unsigned int> >::iterator sit = sub.find(*it);
			if(mit != coms.end() && sit != sub.end())
			{
				mit->second.push_back(i);
				vector<unsigned int> tmp;
				set_intersection(sit->second.begin(), sit->second.end(), ids.begin(), ids.end(), back_inserter(tmp));
				
				sit->second = tmp;

			}else
			{
				vector<unsigned int> tmp;
				tmp.push_back(i);
				coms.insert(map<unsigned int, vector<unsigned int> >::value_type(*it, tmp));

				sub.insert(map<unsigned int, vector<unsigned int> >::value_type(*it, ids));
			}
		}
	}

	
	
	map<unsigned int, vector<unsigned int> >::iterator subIt ;
	for(subIt = sub.begin(); subIt != sub.end(); subIt++)
	{
		sort(subIt->second.begin(), subIt->second.end());
	}
	

	for(subIt = sub.begin(); subIt != sub.end(); subIt++)
	{
		sort(subIt->second.begin(), subIt->second.end());
		if(subIt->second.size()>1)
		{
			vector<unsigned int>::iterator tmp;
			for(tmp=subIt->second.begin(); tmp != subIt->second.end(); tmp ++)
			{
				if(*tmp != subIt->first)
				{
					vector<unsigned int>::iterator finder = lower_bound(sub[*tmp].begin(), sub[*tmp].end(), subIt->first);
					if(finder != sub[*tmp].end() && *finder==subIt->first)
					{
						sub[*tmp].erase(finder);
					}
				}
			}
			coms.erase(subIt->first);
		}
	}

	
	

}


void Algorithm::idl(vector< map<unsigned int, double> >& l, vector<unsigned int>& ids)
{
	ids.clear();
	for(vector< map<unsigned int, double> >::iterator it = l.begin(); it != l.end(); it++)
		this->idx((*it), ids);
}
	
void Algorithm::idx(map<unsigned int, double>& lx, vector<unsigned int>& ids)
{
	for(map<unsigned int, double>::iterator destxi = lx.begin(); destxi != lx.end(); destxi++)
	{
		vector<unsigned int>::iterator it = find(ids.begin(), ids.end(), destxi->first);
		if(it == ids.end())
			ids.push_back(destxi->first);
	}
}

void Algorithm::idl_set(vector< map<unsigned int, double> >& l, vector<unsigned int>& ids)
{
	ids.clear();
	for(vector< map<unsigned int, double> >::iterator it = l.begin(); it != l.end(); it++)
		this->idx_set((*it), ids);
}
	
void Algorithm::idx_set(map<unsigned int, double>& lx, vector<unsigned int>& ids)
{
	for(map<unsigned int, double>::iterator destxi = lx.begin(); destxi != lx.end(); destxi++)
	{
		this->idnew_set.insert(destxi->first);
		//vector<unsigned int>::iterator it = find(ids.begin(), ids.end(), destxi->first);
		//if(it == ids.end())
		//	ids.push_back(destxi->first);
	}
}	

void Algorithm::idl_vector(vector< map<unsigned int, double> >& l, vector<bool>& ids)
{
	//ids.clear();
	for(vector< map<unsigned int, double> >::iterator it = l.begin(); it != l.end(); it++)
		this->idx_vector((*it), ids);
}
	
void Algorithm::idx_vector(map<unsigned int, double>& lx, vector<bool>& ids)
{
	for(map<unsigned int, double>::iterator destxi = lx.begin(); destxi != lx.end(); destxi++)
	{
		ids[destxi->first]=true;
	}
}
	
void Algorithm::count()
{
	counts.clear();
	for(vector< map<unsigned int, double> >::iterator it = dest.begin(); it != dest.end(); it++)
	{
		map<unsigned int, double> mp = (*it);
		for(map<unsigned int, double>::iterator mit = mp.begin(); mit != mp.end(); mit++)
		{
			map<unsigned int, int>::iterator cit = this->counts.find(mit->first);
			if( cit != this->counts.end() )
				cit->second += 1;
			else
				this->counts.insert(map<unsigned int, int>::value_type(mit->first, 1));
		}
	}
}

void Algorithm::count_parallel()
{
	counts.clear();
	for(vector< map<unsigned int, double> >::iterator it = dest.begin(); it != dest.end(); it++)
	{
		map<unsigned int, double> mp = (*it);
		for(map<unsigned int, double>::iterator mit = mp.begin(); mit != mp.end(); mit++)
		{
			map<unsigned int, int>::iterator cit = this->counts.find(mit->first);
			if( cit != this->counts.end() )
				cit->second += 1;
			else
				this->counts.insert(map<unsigned int, int>::value_type(mit->first, 1));
		}
	}
}

void Algorithm::mc(map<unsigned int, int>& newmin)
{
	map<unsigned int, int> cs;
	cs.clear();

	for(map<unsigned int, int>::iterator mit = min.begin(); mit != min.end(); mit++)
	{
		map<unsigned int, int>::iterator cit = this->counts.find(mit->first);
		if( cit != this->counts.end() )
		{
			int i = mit->second < cit->second ? mit->second : cit->second;
			newmin.insert(map<unsigned int, int>::value_type(mit->first, i));
		}
	}
}

//dfs algorithem for disconnected components
static int cnt = 0;
unsigned int DFS(map<unsigned int, bool> &visit, Graph *g, unsigned int vertex, vector<unsigned int> &component)
{

	unsigned int visit_count = 1;
	visit[vertex] = true;
	vector<unsigned int>::iterator it;
	for(it = g->links[vertex].begin(); it != g->links[vertex].end(); it++)
	{
		map<unsigned int, bool>::iterator find = visit.find(*it);
		if(find != visit.end() && visit[*it]==false)
		{
			component.push_back(*it);

			visit_count += DFS(visit, g, *it, component);

		}
	}
	return visit_count;
}

void Algorithm::splitDisconnectComs()
{
	map <unsigned int, vector<unsigned int> > tmpComs;//for temp coms
	set <unsigned int> tmpDel ;//for com to be deleted; 
	map < unsigned int, vector<unsigned int> >::iterator it ;
	unsigned int unusedL = g->links.size();

	for(it = coms.begin(); it != coms.end(); it++)
	{
		
		map<unsigned int, bool> visit;

		//Initialize
		vector<unsigned int>::iterator node;
		for(node = it->second.begin(); node != it->second.end(); node++)
		{
			visit.insert(map<unsigned int, bool>::value_type(*node, false));
		}

		
		for(node = it->second.begin(); node != it->second.end(); node++)
		{
			if(visit[*node] == false)
			{
				vector<unsigned int> component;
				component.push_back(*node);

				if(DFS(visit, g, *node, component)<it->second.size())
				{
					tmpComs.insert(map< unsigned int, vector<unsigned int> >::value_type(unusedL, component));
					tmpDel.insert(it->first);
					unusedL++;
				}else
				{
					break;
				}
				
			}
		}	
	}
	
	set<unsigned int>::iterator delIt;
	for(delIt = tmpDel.begin(); delIt != tmpDel.end(); delIt++)
	{
		coms.erase(*delIt);
	}

	
	map< unsigned int, vector<unsigned int> >::iterator tmpIt;
	for(tmpIt = tmpComs.begin(); tmpIt != tmpComs.end(); tmpIt++)
	{
		coms.insert(map< unsigned int, vector<unsigned int> >::value_type(tmpIt->first, tmpIt->second));
	}
	
}

void Algorithm::saveResult(string filepath, string type)
{
	ofstream file(filepath.c_str());
	if(file)
	{
		if(type=="c2n")
		{
			map< unsigned int, vector<unsigned int> >::iterator it;
			for(it=coms.begin();it!=coms.end();it++)
			{
				vector<unsigned int>::iterator vertex;
				for(vertex=it->second.begin();vertex!=it->second.end();vertex++)
				{
					file<<g->converseNodeMap[*vertex]<<" ";
				}
				file<<endl;
				file.flush();
			}
		}else if(type=="n2c")
		{
			vector< vector<unsigned int> > n2c(g->nb_nodes);
			map< unsigned int, vector<unsigned int> >::iterator it;
			for(it=coms.begin();it!=coms.end();it++)
			{
				vector<unsigned int>::iterator vertex;
				for(vertex=it->second.begin();vertex!=it->second.end();vertex++)
				{
					n2c[*vertex].push_back(it->first);
				}			
			}

			for(int i=0; i<g->nb_nodes; i++)
			{
				file << g->converseNodeMap[i];
				vector<unsigned int>::iterator cIt;
				for(cIt=n2c[i].begin();cIt!=n2c[i].end();cIt++)
				{
					file << " " << *cIt;
				}
				file << endl;
				file.flush();
			}

		}else
		{
			cerr<<"Please give the correct file type, c2n or n2c, please!"<<endl;
		}
		file.close();

	}else
	{
		cerr<<"Cannot open file "<<filepath<<"."<<endl;
	}
}

void Algorithm::saveTime(string filepath,string content,char *inputfile)
{
	ofstream file(filepath.c_str());
	if(file)
	{
		file<<inputfile<<endl;
		file<<content<<endl;
		file.flush();
		file.close();
	}else
	{
		cerr<<"Cannot open file "<<filepath<<"."<<endl;
	}

}

void Algorithm::setp(double p)
{
	this->p = p;
}


