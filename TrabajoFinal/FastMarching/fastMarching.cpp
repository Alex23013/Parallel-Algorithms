#include "fastMarching.hpp"

bool cFastMarching::myfunction (cCell i,cCell j) { return (i.t<j.t); }
bool cFastMarching::myfunction1 (cCell* i,cCell* j) { return (i->t<j->t); }
float cFastMarching::myMin(float a, float b){

	if(a<b) return a;
	return b;
}

cFastMarching::cFastMarching (){};

cFastMarching::cFastMarching (int w, int h){

	m_grid = cGrid (w,h);
	cout<<"OJO "<<m_grid.w<<endl;
	cout<<"OJO "<<m_grid.grid[0][0].neightbours.size()<<endl;
	//FM(ini_i, ini_j);
}

double cFastMarching::solveEikonal(cCell c){

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

bool cFastMarching::FM(int ini_i, int ini_j, int fin_i, int fin_j, int radio){

	//mNarrowBand.clear();
	while (!mNarrowBand.empty())
	{
		mNarrowBand.pop();
	}
	visited.clear();
	
	cout<<"VIVO"<<endl;
	
	m_grid.grid[ini_i][ini_j].t = 0;
	m_grid.grid[ini_i][ini_j].state = "FROZEN";

	
	//chequear aqui!!
	cout<<m_grid.grid[ini_i][ini_j].neightbours.size()<<endl;
	for(int i=0; i<m_grid.grid[ini_i][ini_j].neightbours.size(); ++i){

		cCell *n = m_grid.grid[ini_i][ini_j].neightbours[i];
		cout<<"state: "<<n->state<<endl;
		if( n->state!="FROZEN" && !n->obstacle){
			n->t = solveEikonal(*n);
			if(n->state == "UNKNOWN"){
				n->state = "NARROW BAND";
				
				//mNarrowBand.push_back(*n);
				mNarrowBand.push(*n);
				//sort(mNarrowBand.begin(), mNarrowBand.end(), myfunction);
			}
		}
	}
	while(!mNarrowBand.empty()){

		//cCell actual = mNarrowBand[0];
		cCell actual = mNarrowBand.top();
		
		visited.push_back(actual);
		//cout<<"Cell nro: "<<visited.size()<<endl;
		
		//mNarrowBand.erase(mNarrowBand.begin());
		mNarrowBand.pop();
		
		m_grid.grid[actual.i][actual.j].state = "FROZEN";
		if(actual.i==fin_i && actual.j==fin_j )
			return true;
		if(radio>0 && (sqrt(pow(actual.i-ini_i,2)+pow(actual.j-ini_j,2))>=radio))
			return false;
		//clock_t begin = clock();
		for(int i=0; i<actual.neightbours.size(); ++i){

			cCell *n = actual.neightbours[i];
			if( n->state!="FROZEN" && !n->obstacle){
				n->t = solveEikonal(*n);
				if(n->state == "UNKNOWN"){
					n->state = "NARROW BAND";
					
					//mNarrowBand.push_back(*n);
					mNarrowBand.push(*n);
					//sort(mNarrowBand.begin(), mNarrowBand.end(), myfunction);
				}
			}
		}
		
	}
	return true;
}

void cFastMarching::genPath(int x, int y){

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
		sort(n.begin(), n.end(), myfunction1);
		actual=n[0];
		path.push_back(actual);
		//cout<<"("<<actual->i<<","<<actual->j<<"), ";
	}

}
