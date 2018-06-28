#ifndef FAST_ITERATIVE_H
#define FAST_ITERATIVE_H

#include <iostream>
#include <math.h>
#include <algorithm>
#include <list>
#include <queue>
#include <stdlib.h> 
#include <thread>
#include <mutex>
//#include "grilla_FI.hpp"
#include "../FastMarching/grilla_FM.hpp"

using namespace std;

class cFastIterative {

	public:

		cGrid m_grid;
		
		list<cCell> mNarrowBand;
		vector<cCell> visited;
		vector<cCell*> path;
        vector<vector<cCell> > nb;

		
		double error;

        //Empty constructor
		cFastIterative ();

        //Cosntructor with the size of the grid
		cFastIterative (int, int);

        //Solve the eikonal of the given cell
		double solveEikonal(cCell);

        int num_threads;
        //Do the magic given the start and end indexes
		bool FI(int, int, int, int, int);

        void FIMParallel(list<cCell>::iterator, mutex* myMutex, int);
        //Extract the path using gradient descent
		void genPath(int, int);
		void printNarrow();
		
		void parallelJoinMaster(int );
		void parallelJoin(int , int );
		void join2Vector(vector<cCell>* v1,vector<cCell>* v2);

        //Functions for sorting lists
		static bool myfunction (cCell i,cCell j);
		static bool myfunction1 (cCell* i,cCell* j);

        //Gives the min of two floats
		float myMin(float a, float b);

};

#endif
