// File: graph_unweighted.cpp


#include "graph_unweighted.h"

using namespace std;

Graph::Graph(const char *filename ,bool weight_if){
  ifstream finput;
  finput.open(filename,fstream::in);
  
  int weightWarning=0;
  string line;
  int lineNum=1;
  char split=',';
  char split_space=' ';
  char split_tab='	';
  int pos=-1;
  int splitFlag=0;
  if(getline(finput,line)){
	  pos = line.find_first_of(split);
	  if(pos==-1)
		  splitFlag=1;
  }
  do{

      //cout<<line<<endl;
	  string char1,char2;
	  unsigned int src, dest;
	  double weight=1.0;
	  if(splitFlag==0){
		  pos=line.find_first_of(split);
		  if(pos!=-1){
			  if(pos==0){
				  cerr<<"Data error, line "<<lineNum<<" lack first node"<<endl;
				  exit(0);
			  }
			  if(pos==line.size()-1){
				  cerr<<"Data error, line "<<lineNum<<" lack second node"<<endl;
				  exit(0);
			  }
			  char1=line.substr(0,pos);

			  if(weight_if==true){
				  line=line.substr(pos+1);
				  pos=-1;
				  pos=line.find_first_of(split);
				  if(pos==-1)
				  {
					  cerr<<"Data error, line "<<lineNum<<" not weighted format"<<endl;
					  exit(0);
				  }else if(pos==line.size()-1){
					  cerr<<"Data error, line "<<lineNum<<" lack weight"<<endl;
					  exit(0);
				  }
				  char2=line.substr(0,pos);

				  weight=atof(line.substr(pos+1).c_str());
				  if(weight==0){
					  string weightString=line.substr(pos+1);
					  for(int j=0; j<weightString.size(); j++){
						  if(!(weightString[j]=='0'|| weightString[j]=='.')){
							  cerr<<"Data error, line "<<lineNum<<" weight error"<<endl;
							  exit(0);
						  }
					  }
				  }

			  }else{
				  char2=line.substr(pos+1);
				  pos=-1;
				  pos=char2.find(split);
				  if(pos!=-1){
					  if(weightWarning==0){
						  cerr<<"!!!Data warning, line "<<lineNum<<" may be a weighted format data"<<endl;
						  weightWarning++;
					  }

				  }

			  }

		  }else{
			  cerr<<"Data error, line "<<lineNum<<" lack split symbol"<<endl;
			  exit(0);
		  }
	  }else{
		  pos=0;
		  while(pos<line.size() && line[pos]!=split_space && line[pos]!=split_tab)
			  pos++;
		  if(pos==0){
			  cerr<<"Data error, line "<<lineNum<<" lack first node"<<endl;
			  exit(0);
		  }
		  char1=line.substr(0,pos);
                  //cout<<"char1 = "<<char1<<endl;
		  line=line.substr(pos);
		  if(line==""){
			  cerr<<"Data error, line "<<lineNum<<" lack split symbol"<<endl;
			  exit(0);
		  }
		  pos=0;
		  while(pos<line.size() && (line[pos]==split_space || line[pos]==split_tab))
			  pos++;
		  line=line.substr(pos);
		  if(line==""){
			  cerr<<"Data error, line "<<lineNum<<" lack second node"<<endl;
			  exit(0);
		  }
		  if(weight_if==true){
			  pos=0;
			  while(pos<line.size() && line[pos]!=split_space && line[pos]!=split_tab)
				  pos++;
			  if(pos==line.size()){
				  cerr<<"Data error, line "<<lineNum<<" not weighted format"<<endl;
				  exit(0);
			  }
			  char2=line.substr(0,pos);
			  line=line.substr(pos);

			  pos=0;
			  while(pos<line.size() && (line[pos]==split_space || line[pos]==split_tab))
				  pos++;
			  if(pos==line.size()){
				  cerr<<"Data error, line "<<lineNum<<" lack weight"<<endl;
				  exit(0);
			  }
			  line=line.substr(pos);
	
			  weight=atof(line.c_str());
			  if(weight==0){
				  for(int j=0; j<line.size(); j++){
					  if(!(line[j]=='0'|| line[j]=='.')){
						  cerr<<"Data error, line "<<lineNum<<" weight error"<<endl;
						  exit(0);
					  }
				  }
			  }
		  }else{
			  char2=line;
			  int pos1=-1,pos2=-1;
			  pos1=char2.find(split_space);
			  pos2=char2.find(split_tab);
			  if(pos1!=-1 || pos2!=-1){
				  if(weightWarning==0){
					  cerr<<"!!!Data warning, line "<<lineNum<<"  may be a weighted format data"<<endl;
					  weightWarning++;
				  }

			  }
		  }
	  }
          int pos5 = -1;
          pos5 = char2.find_first_of('\n');
          int pos6 = -1;
          pos6 = char2.find_first_of('\r');
          int minpos = 1000;
          if(pos5 != -1 && pos6 != -1)
          {

              if(pos5 < pos6)
                  minpos = pos5;
              else
                  minpos = pos6;
          }else if(pos5 == -1 && pos6 != -1)
          {
              minpos = pos6;
          }else if(pos5 != -1 && pos6 == -1)
          {
              minpos = pos5;
          }else{

              minpos = char2.size();

          }

          char2 = char2.substr(0,minpos);

          //cout<<"char1: "<<char1<<", char1.size = "<<char1.size()<<endl;
          //cout<<"char2: "<<char2<<", char2.size = "<<char2.size()<<endl;

	  if(char1==char2)
		  continue;
	  map<string,unsigned int>::iterator it1,it2;
	  it1=nodeMap.find(char1);
	  if(it1!=nodeMap.end()){
		  src=it1->second;
	  }else{
		  src=nodeMap.size();
		  nodeMap.insert(make_pair(char1,src));
		  converseNodeMap.insert(make_pair(src,char1));
	  }
	  it2=nodeMap.find(char2);
	  if (it2!=nodeMap.end())
	  {
		  dest=it2->second;
	  }else{
		  dest=nodeMap.size();
		  nodeMap.insert(make_pair(char2,dest));
		  converseNodeMap.insert(make_pair(dest,char2));
	  }

	  if(src==dest)
		  continue;
	  if (links.size()<=max(src,dest)+1)
		links.resize(max(src,dest)+1);
	  
	  links[src].push_back(dest);
	  links[dest].push_back(src);
	  lineNum++;
  }while(getline(finput,line));


  for (unsigned int i=0 ; i<links.size() ; i++) {      
	  set<unsigned int> m;
	  set<unsigned int>::iterator it;

	  for (unsigned int j=0 ; j<links[i].size() ; j++) {
		  it = m.find(links[i][j]);
		  if (it==m.end())
			  m.insert(links[i][j]);
	  }

	  vector<unsigned int> v;
	  for (it = m.begin() ; it!=m.end() ; it++){
		  v.push_back(*it);

	  }
	  links[i].clear();
	  links[i]=v;
  }
  nb_links=0;
  for(unsigned int i=0; i<links.size();i++){
	  nb_links+=links[i].size();
	  sort(links[i].begin(),links[i].end());
  }
  nb_links=nb_links/2;
  nb_nodes=links.size();
  finput.close();
}
 
