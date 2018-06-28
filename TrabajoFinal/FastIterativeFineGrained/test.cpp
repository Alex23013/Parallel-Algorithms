#include <cstdlib>
#include <iostream>
#include <random>
#include <time.h>
#include <fstream>
#include <GL/gl.h>
#include <GL/glut.h>

//#include "grilla_FI.hpp"
#include "fast_iterative.hpp"

#define COLUMNS 1000
#define ROWS 1000

int main(int argc, char** argv) {

    int inix, iniy, goalx, goaly;

    inix=0;
    iniy=0;
    goalx=ROWS-1;
    goaly=COLUMNS-1;
        cout<<"\nFIM LIST "<<COLUMNS<<" X "<<ROWS;

    for(int i=0; i<1; ++i){

        bool pathFound;

        vector<cCell> visited, path;

        cFastIterative fastIterative = cFastIterative (COLUMNS, ROWS);;
        
        clock_t begin = clock();
        pathFound = fastIterative.FI(goalx,goaly, inix, iniy,-1);
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        //cout<<"Time: "<<elapsed_secs<<endl;
        cout<<endl<<elapsed_secs;
        if(pathFound) fastIterative.genPath(inix,iniy);
        visited = fastIterative.visited;
        //cout<<"Cells: "<<visited.size()<<endl<<endl;
    }
}
