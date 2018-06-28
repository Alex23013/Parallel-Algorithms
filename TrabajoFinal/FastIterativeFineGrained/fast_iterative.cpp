#include "fast_iterative.hpp"

bool cFastIterative::myfunction (cCell i,cCell j) { return (i.t<j.t); }
bool cFastIterative::myfunction1 (cCell* i,cCell* j) { return (i->t<j->t); }
float cFastIterative::myMin(float a, float b){
	if(a<b) return a;
	return b;
}

cFastIterative::cFastIterative (){};

cFastIterative::cFastIterative (int w, int h){
	m_grid = cGrid (w,h);
	error = 0.01;
	num_threads = 4;
	//cout<<"Width "<<m_grid.w<<endl;
	//cout<<"Height "<<m_grid.h<<endl;
}

double cFastIterative::solveEikonal(cCell c){

	double t1,t2, T;

	if(c.i-1<0) t1=m_grid.grid[c.i+1][c.j].t;

	else if(c.i+1>=m_grid.h) t1=m_grid.grid[c.i-1][c.j].t;

	else t1=myMin(m_grid.grid[c.i+1][c.j].t, m_grid.grid[c.i-1][c.j].t);

	if(c.j-1<0) t2=m_grid.grid[c.i][c.j+1].t;

	else if(c.j+1>=m_grid.w) t2=m_grid.grid[c.i][c.j-1].t;

	else t2=myMin(m_grid.grid[c.i][c.j+1].t,m_grid.grid[c.i][c.j-1].t);

	if(abs(t1-t2)<1/c.vel){
		T = (t1+t2 + pow((2/c.vel-pow((t1-t2),2)),0.5))/2;
	}
	else{
		T = min(t1,t2) + 1/c.vel;
	}
	//cout<<"T: "<<T<<endl;
	return T;
}


void cFastIterative::FIMParallel(list<cCell>::iterator i, mutex* myMutex, int cont){
       
    
   ///cout<<"PARALLEL\n";
   
    cCell *actual = &m_grid.grid[(*i).i][(*i).j];
    myMutex->lock(); 
    visited.push_back(*actual);
    myMutex->unlock();
   ///cout<<"actualneir: ("<<(*i).i<<","<<(*i).j<<")"<<actual->neightbours.size()<<endl; 
    double tim,tjm; //ti~ , tj~ 	
	tim = actual->t;
	actual->t = solveEikonal(*actual);
	///cout<<"fffff\n";
	//if(fabs(actual->t - tim) < error){
	   /// cout<<"actualneir: ("<<(*i).i<<","<<(*i).j<<")"<<actual->neightbours.size();
	   /// 	cout<<" mNarrowBand: "<<mNarrowBand.size()<<endl;
	    for(int j=0; j<actual->neightbours.size(); ++j){
            cCell *n = actual->neightbours[j];
	        
		    if(n->state != "NARROW BAND" && !n->obstacle ){
                
	            tjm = solveEikonal(*n);

	            if(tjm + numeric_limits<double>::epsilon() < n->t){
                    
                    //myMutex->lock();                    
                    n->t = tjm;  
                    n->state = "NARROW BAND";
                    
                    nb[cont].push_back(*n);
				    //mNarrowBand.insert(i,*n);
				    //myMutex->unlock();
				    
 	            }
 	        }
 	    }
    //i=mNarrowBand.erase(i);
   //--i;
   //i--;
    actual->state = "FROZEN";
    //}
    //myMutex->unlock();
    return;
}

void cFastIterative:: join2Vector(vector<cCell>* v1,vector<cCell>* v2){
    v1->insert(v1->end(), v2->begin(), v2->end());
}

void cFastIterative:: parallelJoin(int level, int round){
    thread bucket[level];

    for(int i = 0 ;i < level; i++){
       // cout<<"join:"<<round*i<<" & "<<(i*round)+(round/2)<<endl;
                    //bucket[cont] = thread(&cFastIterative::FIMParallel,this,i,&myMutex,cont);
        bucket[i] = thread(&cFastIterative::join2Vector, this,&nb[round*i], &nb[(i*round)+(round/2)]);
    }

    for(int i=0; i<level; ++i){
        bucket[i].join();
    }

}


void cFastIterative:: parallelJoinMaster(int size){
    int cont=2;
    while(size>1){
    
        //cout<<"size: "<<size;
        //cout<<" cont: "<<cont<<endl;
        parallelJoin(size/2, cont);
//        cont=cont*2;
        cont=cont<<1;
        
        if(size%2){
            size=ceil(size/2 +1);
        }else{
            size=ceil(size/2);
        }
    }
}
void cFastIterative:: printNarrow(){
    cout<<"\nNBand: ";
    for(list<cCell>::iterator i = mNarrowBand.begin(); i!=mNarrowBand.end(); ++i){
        cout<<"("<<(*i).i<<","<<(*i).j<<")-";
    }
}

bool cFastIterative::FI(int ini_i, int ini_j, int fin_i, int fin_j, int radio){

	mNarrowBand.clear();
	visited.clear();
	m_grid.grid[ini_i][ini_j].t = 0;
	m_grid.grid[ini_i][ini_j].state = "NARROW BAND";
  
    //Initializing

	cCell *n = &m_grid.grid[ini_i][ini_j];
	n->t = 0;
	for(int j=0; j<(*n).neightbours.size(); ++j){
		cCell *nn = n->neightbours[j];

		nn->state = "FROZEN";
		mNarrowBand.push_back(*nn);

	}



	//Wave expansion

    mutex myMutex;
    
    int uu =0;
    while(!mNarrowBand.empty()){
//    	printNarrow(); //to see what its on NarrowBand
		int size = mNarrowBand.size();
		nb.clear();
        nb.resize(size);
        thread bucket[size];
        int cont = 0;
               ///cout<<"aaaaaaaaa: "<<mNarrowBand.size()<<endl;
        for(list<cCell>::iterator i = mNarrowBand.begin(); i!=mNarrowBand.end(); ++i){
            
            //cout<<" th["<<cont<<"] ";
            bucket[cont] = thread(&cFastIterative::FIMParallel,this,i,&myMutex,cont);
            //FIMParallel(i,&myMutex);

          //--i;
            cont++;
        }

        for(int i=0; i<size; ++i){

            bucket[i].join();
        }
        mNarrowBand.clear();

  /*      for(int i=0; i<size; ++i){
            for(int j=0; j<nb[i].size(); ++j){
                mNarrowBand.insert(mNarrowBand.begin(), nb[i][j]);        
            }
        }
*/        
        

        for(int i=0; i<size; ++i){
           //    std::copy(nb[i].begin(), nb[i].end(), std::back_inserter( mNarrowBand ) );
         //     mNarrowBand.insert(mNarrowBand.begin(), nb[i].begin(), nb[i].end()); //mNarrowBand.end() //slowy and .begin//faster
           mNarrowBand.assign(nb[i].begin(), nb[i].end());
        
        }
        
        //Parallel Join
        
           // parallelJoinMaster(size);   
    //       mNarrowBand.assign(nb[0].begin(), nb[0].end());
            //cout<<"\nCOMPLETE size"<<mNarrowBand.size()<<endl;
             uu++;    
    }
    
  return true;
}

void cFastIterative::genPath(int x, int y){

	path.clear();
	cCell *actual = &m_grid.grid[x][y];
	

	while(actual->t!=0){

		//cout<<"entro "<<actual->t<<endl;
		vector<cCell*> n;
		if(actual->i+1<m_grid.h)
			n.push_back(&m_grid.grid[actual->i+1][actual->j]);
		if(actual->i-1>=0)
			n.push_back(&m_grid.grid[actual->i-1][actual->j]);
		if(actual->j+1<m_grid.w)
			n.push_back(&m_grid.grid[actual->i][actual->j+1]);
		if(actual->j-1>=0)
			n.push_back(&m_grid.grid[actual->i][actual->j-1]);
		//FOR NOW
		if(actual->i-1>=0 && actual->j-1>=0){
			n.push_back(&m_grid.grid[actual->i-1][actual->j-1]);
		}
		if(actual->i+1<m_grid.h && actual->j+1<m_grid.w){
			n.push_back(&m_grid.grid[actual->i+1][actual->j+1]);
		}
		if(actual->i-1>=0 && actual->j+1<m_grid.w){
			n.push_back(&m_grid.grid[actual->i-1][actual->j+1]);
		}
		if(actual->i+1<m_grid.h && actual->j-1>=0){
			n.push_back(&m_grid.grid[actual->i+1][actual->j-1]);
		}
		sort(n.begin(), n.end(), myfunction1);
		actual=n[0];
		path.push_back(actual);
		//cout<<"("<<actual->i<<","<<actual->j<<"), ";
	}

}
