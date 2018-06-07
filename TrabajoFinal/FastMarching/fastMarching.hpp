#ifndef FASTMARCHING_H
#define FASTMARCHING_H

#include <iostream>
#include <math.h>
#include <algorithm>
#include <queue> 

#include "grilla_FM.hpp"
//#include "../A_star/grilla_A_star.hpp"

using namespace std;

class mycomparison
{
  bool reverse;
public:
  mycomparison(const bool& revparam=false)
    {reverse=revparam;}
  bool operator() (cCell& lhs, cCell& rhs) const
  {
    if (reverse) return (lhs.t >rhs.t);
    else return (lhs.t >rhs.t);
  }
};

class cFastMarching {

	public:
		cGrid m_grid;
		
		//vector<cCell> mNarrowBand;
		priority_queue<cCell,std::vector<cCell>,mycomparison> mNarrowBand;
		vector<cCell> visited;
		vector<cCell*> path;
		cFastMarching ();
		cFastMarching (int, int);
		double solveEikonal(cCell);
		//void init();
		bool FM(int, int, int, int, int);
		void genPath(int, int);
		static bool myfunction (cCell i,cCell j);
		static bool myfunction1 (cCell* i,cCell* j);
		float myMin(float a, float b);

};

#endif
