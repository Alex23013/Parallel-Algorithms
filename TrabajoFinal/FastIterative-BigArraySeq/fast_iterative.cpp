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
	arrayLength = w*h+1000;
	start = 0;
    end = 0;
    mNarrowArray = new cCell* [arrayLength];
    insertPoint = 0;
    ///for(int i = 0; i< arrayLength;i++){
    ///    mNarrowArray[i]= NULL;
    ///}
    ///cout<<"Iniciado\n";
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

void cFastIterative:: printNarrow(){
    cout<<"\nNBand: ";
    for(list<cCell>::iterator i = mNarrowBand.begin(); i!=mNarrowBand.end(); ++i){
        cout<<"("<<(*i).i<<","<<(*i).j<<")-";
    }
}

void cFastIterative:: printNarrowArray(){
    cout<<"\nNBandArray: ";
    for(int k = 0; k < end; ++k){
//        cout<<mNarrowArray[arrayLength-1-i]<<" ";
         cout<<"("<<mNarrowArray[arrayLength-1-k]->i<<","<<mNarrowArray[arrayLength-1-k]->j<<")-";
    }
    cout<<endl;
}

void cFastIterative:: printNarrowArrayActual(){
    cout<<"\nNBandArray actual: ";
    for(int k = start; k < end; ++k){
//        cout<<mNarrowArray[arrayLength-1-i]<<" ";
         cout<<"("<<mNarrowArray[arrayLength-1-k]->i<<","<<mNarrowArray[arrayLength-1-k]->j<<")-";
    }
    cout<<endl;
}

void cFastIterative:: FIMParallel(int k,mutex* myMutex){
        double tim,tjm; //ti~ , tj~ 
cCell *actual = &m_grid.grid[mNarrowArray[arrayLength-1-k]->i][mNarrowArray[arrayLength-1-k]->j];
//            cout<<"\nk: "<<k<<"thread: I: "<<actual->i<<" J:"<<actual->j<<endl;
            visited.push_back(*actual);
            tim = actual->t;
        	actual->t = solveEikonal(*actual);
        	//cout<<actual->t<<"-"<<tim<<endl;
            if(fabs(actual->t - tim) < error){
              //  cout<<" f1 ";
        	   for(int j=0; j<actual->neightbours.size(); ++j){
				//	         cout<<" f2 ";
        	        cCell *n = actual->neightbours[j];
        	        
					if(n->state != "NARROW BAND" && !n->obstacle ){
                  //                     cout<<" f3 "; 
        	            tjm = solveEikonal(*n);

        	            if(tjm + numeric_limits<double>::epsilon() < n->t){
                            n->t = tjm;  
                            n->state = "NARROW BAND";
                    //                     cout<<" f4 ";  
                                                                           
							///myMutex->lock();
							mNarrowArray[arrayLength-1-insertPoint] = n;    

                            ++insertPoint;
                      //      cout<<"IP:"<<insertPoint;
                           // ++end;        
                        ///    myMutex->unlock();                    
         	              }
         	        }
         	                           
         	    }
               /// myMutex->lock();                    
         	        ++start;
         	   /// myMutex->unlock(); 
        	    actual->state = "FROZEN";
        	  //  if(actual->i==fin_i && actual->j==fin_j )
			  //     return true;
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
        mNarrowArray[arrayLength-1-j] = nn;    
        ++insertPoint;

	}
    end=insertPoint;     
    int dif = end - start; 
    mutex myMutex;
    while(dif>0){
        //printNarrowArray();
        //printNarrowArrayActual();
        //cout<<"\nsizeNarrowArray ["<<dif<<"]";
        ///thread bucket[dif];
        int cont = 0;
        for(int k = start;k<end;++k){
            
           /// bucket[cont] = thread(&cFastIterative::FIMParallel,this,k,&myMutex);
            cont++;
            FIMParallel(k,&myMutex);
        	
        }
      
        /*for(int i=0; i<dif; ++i){

            bucket[i].join();
        }*/
        
        //cout<<" iP:"<<insertPoint;
        end=insertPoint; 
        
        
        //cout<<"start "<<start<<" end"<<end<<endl;
        dif =end-start;        
    }
    return true;
    /*
    cout<<"FIM sequential:\n";
    while(!mNarrowBand.empty()){
            	//printNarrow();
        for(list<cCell>::iterator i = mNarrowBand.begin(); i!=mNarrowBand.end(); ++i){

            cCell *actual = &m_grid.grid[(*i).i][(*i).j]; //
            visited.push_back(*actual);
        	
            //printf("actual:[%d,%d]\n", actual->i, actual->j);
        	tim = actual->t;
        	actual->t = solveEikonal(*actual);
        	
        	/*DEBUG
			for(list<cCell>::iterator j = mNarrowBand.begin(); j!=mNarrowBand.end(); ++j){
            
                printf("[%d,%d],", (*j).i, (*j).j);
            }
            cout<<endl;
            m_grid.print();
            int algo;
            cin>>algo;
			
        	//cout<<" tim: "<<tim<<" actualT: "<<actual.t<<" dif "<<abs(actual.t - tim)<<endl;
        	cout<<actual->t<<"-"<<tim<<endl;
        	if(fabs(actual->t - tim) < error){
        	    //cout<<"converge";

        	    for(int j=0; j<actual->neightbours.size(); ++j){
					
        	        cCell *n = actual->neightbours[j];
        	        
					if(n->state != "NARROW BAND" && !n->obstacle ){

        	            tjm = solveEikonal(*n);

        	            if(tjm + numeric_limits<double>::epsilon() < n->t){
                            n->t = tjm;  
                            n->state = "NARROW BAND";

							mNarrowBand.insert(i,*n);
         	            }
         	        }
         	    }

				i=mNarrowBand.erase(i);
				--i;
				//printf("it:[%d,%d]\n", (*i).i, (*i).j);

        	    actual->state = "FROZEN";
        	    if(actual->i==fin_i && actual->j==fin_j )
			        return true;
				if(radio>0 && (sqrt(pow(actual->i-ini_i,2)+pow(actual->j-ini_j,2))>=radio))
					return false;
        	 }
        }
    }
    */
  //If I connot arrive to goal return false, else return true
  return false;
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
