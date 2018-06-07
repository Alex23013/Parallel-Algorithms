#ifndef GRILLA_FI_H
#define GRILLA_FI_H

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

struct cCell {

    //The velocity
	float vel;

    //The time of arrival of the wave
	double t;

    //The index of the cell
	int i,j;
    
    //A state... just in case
	string state;

    //The neightbours (4 or 8)
	vector <cCell*> neightbours;

    //If the cell is an obstacle or not
	bool obstacle = false;
};

class cGrid {

	public:

        //Matrix of cells
		vector< vector<cCell> > grid;

        //Heigth and with of the grid
		int h, w;

        //Empty constructor
		cGrid (){};

        //Constructor with 2 parameters, 2 dimensions only
		cGrid (int h, int w){

			//Assigning values
            this->h = h;
			this->w = w;

            //Initializing the grid
			init();
		}

		void init(){

			//Creating empty rows
            grid.resize(h);

			for (int i = 0; i < h; i++) {

                //Filling a row with empty cells
				grid[i].resize(w);

                //Initializing cells
				for (int j = 0; j < w; j++) {
					grid[i][j].vel = 1;
					grid[i][j].t = std::numeric_limits<double>::infinity();
					grid[i][j].i = i;
					grid[i][j].j = j;
					grid[i][j].state = "UNKNOWN";
				}
			}

            //Assigning neightbours to each cell only if it is not an obstacle
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					if (!grid[i][j].obstacle) 
						fillNeightbours(&grid[i][j]);
				}
			}	
		}

		void setObstacle(int i, int j){

			//Turning a cell into an obstacle
            grid[i][j].obstacle=true;
		}

		//Filling neightbours with the memory direction of the cell's neightbours
        //4 direction only
        void fillNeightbours (cCell *c){

			c->neightbours.clear();
			
			if(c->i-1>=0) c->neightbours.push_back(&grid[c->i-1][c->j]);

			if(c->i+1<h) c->neightbours.push_back(&grid[c->i+1][c->j]);

			if(c->j-1>=0) c->neightbours.push_back(&grid[c->i][c->j-1]);

			if(c->j+1<w) c->neightbours.push_back(&grid[c->i][c->j+1]);

		}

		//Printing the velocity and the time of each cell
        void print(){

			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					printf("(%.1f,%.4f)", grid[i][j].vel, grid[i][j].t);
				}
				printf("\n");
			}
		}
};

#endif
