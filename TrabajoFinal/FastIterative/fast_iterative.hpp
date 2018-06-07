#ifndef FAST_ITERATIVE_H
#define FAST_ITERATIVE_H

#include <iostream>
#include <math.h>
#include <algorithm>
#include <list>
#include <queue>
#include <stdlib.h> 
//#include "grilla_FI.hpp"
#include "../FastMarching/grilla_FM.hpp"

using namespace std;

class cFastIterative {

	public:

		cGrid m_grid;
		
		list<cCell> mNarrowBand;
		vector<cCell> visited;
		vector<cCell*> path;
		
		double error;

        //Empty constructor
		cFastIterative ();

        //Cosntructor with the size of the grid
		cFastIterative (int, int);

        //Solve the eikonal of the given cell
		double solveEikonal(cCell);

        //Do the magic given the start and end indexes
		bool FI(int, int, int, int, int);

        //Extract the path using gradient descent
		void genPath(int, int);

        //Functions for sorting lists
		static bool myfunction (cCell i,cCell j);
		static bool myfunction1 (cCell* i,cCell* j);

        //Gives the min of two floats
		float myMin(float a, float b);

};

#endif
