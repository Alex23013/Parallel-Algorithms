#ifndef GRILLA_FM_H
#define GRILLA_FM_H

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

struct cCell {

	float vel;
	double t;
	int i,j;
	string state;
	vector <cCell*> neightbours;
	bool obstacle = false;
};

class cGrid {

	public:
		vector< vector<cCell> > grid;
		int h, w;
		cGrid (){};
		cGrid (int h, int w){

			this->h = h;
			this->w = w;

			init();
		}

		void init(){

			grid.resize(h);

			for (int i = 0; i < h; i++) {

				grid[i].resize(w);
				for (int j = 0; j < w; j++) {
					grid[i][j].vel = 1;
					grid[i][j].t = std::numeric_limits<double>::infinity();
					grid[i][j].i = i;
					grid[i][j].j = j;
					grid[i][j].state = "UNKNOWN";
				}
			}

			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					if (!grid[i][j].obstacle) 
						fillNeightbours(&grid[i][j]);
				}
			}	
		}

		void setObstacle(int i, int j){

			grid[i][j].obstacle=true;
		}

		void fillNeightbours (cCell *c){

			c->neightbours.clear();
			
			if(c->i-1>=0) c->neightbours.push_back(&grid[c->i-1][c->j]);

			if(c->i+1<h) c->neightbours.push_back(&grid[c->i+1][c->j]);

			if(c->j-1>=0) c->neightbours.push_back(&grid[c->i][c->j-1]);

			if(c->j+1<w) c->neightbours.push_back(&grid[c->i][c->j+1]);

		}

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