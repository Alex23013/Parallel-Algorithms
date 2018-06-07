#include <cstdlib>
#include <iostream>
#include <random>
#include <time.h>
#include<fstream>
#include <GL/gl.h>
#include <GL/glut.h>

#include "grilla_FM.hpp"
#include "fastMarching.hpp"
//#include "parallelFM.hpp"

#define COLUMNS 100
#define ROWS 100
#define FPS 60

using namespace std;

int it, inix, iniy, goalx, goaly, countClicks, screenHeight;
bool iniSeted, goalSeted, clickRightDown, pathFound;

vector<cCell> visited, path, obstacles;

cFastMarching fastMarching;

void init(){

    glClearColor(0.0,0.0,0.0,1.0);
}

void cell(int x, int y){

    glLineWidth(1.0);
    glColor3f(1.0,1.0,1.0);

    glBegin(GL_LINE_LOOP);
        glVertex2f(x,y);
        glVertex2f(x+1,y);
        glVertex2f(x+1,y+1);
        glVertex2f(x,y+1);
    glEnd();
}

void fillCell(int x, int y){

    glRectd(y,(COLUMNS-1)-x,y+1,(COLUMNS-1)-x+1);
}

void drawGrid(){

    if(iniSeted){
        glColor3f(0.0,1.0,0.0);
        fillCell(inix,iniy);
    }
    if(goalSeted){
        glColor3f(1.0,0.0,0.0);
        fillCell(goalx,goaly);
    }

    for(int i=0; i<obstacles.size(); ++i){

        glColor3f(0.0,0.0,0.5);
        fillCell(obstacles[i].i,obstacles[i].j);
    }
    glColor3f(1.0,1.0,1.0);
    for(int i=0; i<visited.size(); ++i){
        fillCell(visited[i].i,visited[i].j);
    }

    for(int i=0; i<fastMarching.path.size(); ++i){
        glColor3f(1.0,0.0,0.0);
        fillCell(fastMarching.path[i]->i,fastMarching.path[i]->j);
    }
    /*for(int i=0; i<ROWS; ++i){
        
        for(int j=0; j<COLUMNS; ++j){

            cell(i,j);
        }
    }*/
    
}

void display_callback(){

    int x,y;
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    /*if(it<visited.size()){
		 fillCell(visited[it].i,visited[it].j);
		 ++it;
	}
	else{
		for(int i=0; i<fastMarching.path.size(); ++i){
			//cout<<fastMarching.path.size()<<" "<<i<<endl;
            if(fastMarching.path[i]->obstacle){
                cout<<"KHA"<<endl;
                fastMarching.m_grid.init();
                pathFound = fastMarching.FM(goalx, goaly, inix, iniy);
                fastMarching.m_grid.print();
                if(pathFound) fastMarching.genPath(inix,iniy);
                visited = fastMarching.visited;
                //path = fastMarching.path;
                break;
            }
            else
                fillCell(fastMarching.path[i]->i,fastMarching.path[i]->j);
		}
	}*/
    
    //fillCell(x,y);
    //cout<<"here"<<endl;
    glutSwapBuffers();
}

void reshape_callback(int w, int h){

    glViewport(0,0,(GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,COLUMNS,0.0,ROWS,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer_callback(int){

    glutPostRedisplay();
    glutTimerFunc(1000/FPS,timer_callback,0);
}

void mouseClicks(int button, int state, int x, int y) {
    cout<<state<<endl;
    clickRightDown=!state;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        
        if(countClicks==0){
            inix = y/(screenHeight/COLUMNS);
            iniy = x/(screenHeight/COLUMNS);
            iniSeted = true;
        }
        if(countClicks==1){
            goalx = y/(screenHeight/COLUMNS);
            goaly = x/(screenHeight/COLUMNS);
            goalSeted = true;
        }
        if(countClicks>1){
            
                //cout<<state<<endl;
                //fastMarching.m_grid.setObstacle(y/(500/COLUMNS),x/(500/COLUMNS));
                //for(int i=0; i<fastMarching.m_grid.grid.size(); ++i){
                //    for(int j=0; j<fastMarching.m_grid.grid[i].size(); ++j){
                //        if(fastMarching.m_grid.grid[i][j].obstacle)
                //            obstacles.push_back(fastMarching.m_grid.grid[i][j]);
                //    }
                //}
                
        }
        ++countClicks;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        
        clock_t begin = clock();
        pathFound = fastMarching.FM(goalx,goaly, inix, iniy, -1);
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout<<"Time: "<<elapsed_secs<<endl<<endl;
        //fastMarching.m_grid.print();
        if(pathFound) fastMarching.genPath(inix,iniy);
        visited = fastMarching.visited;
        cout<<"Cells: "<<visited.size()<<endl<<endl;
        //path = fastMarching.path;
    }
}

void mouse_motion(int x, int y)
{
    if(countClicks>1 && clickRightDown){
        if((y/(screenHeight/COLUMNS)!=goalx || x/(screenHeight/COLUMNS)!=goaly) && (y/(screenHeight/COLUMNS)!=inix || x/(screenHeight/COLUMNS)!=iniy)) 
            fastMarching.m_grid.setObstacle(y/(screenHeight/COLUMNS),x/(screenHeight/COLUMNS));
        for(int i=0; i<fastMarching.m_grid.grid.size(); ++i){
            for(int j=0; j<fastMarching.m_grid.grid[i].size(); ++j){
                if(fastMarching.m_grid.grid[i][j].obstacle)
                    obstacles.push_back(fastMarching.m_grid.grid[i][j]);
            }
        }

        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {

    countClicks = 0;
    iniSeted = false;
    goalSeted = false;
    clickRightDown = false;
    /*iniSeted = true;
    goalSeted = true;
    clickRightDown = false;
	inix=70;
    iniy=20;
    goalx=40;
    goaly=40;*/
    //cFastMarching fastMarching(COLUMNS, ROWS, inix, iniy);
    
    
    fastMarching = cFastMarching (COLUMNS, ROWS);
	
    /*ifstream myReadFile;
    myReadFile.open("test.txt");
    string  output;
    if (myReadFile.is_open()) {
        int i=0;
        while (!myReadFile.eof()) {

            myReadFile >> output;
            for(int j=0; j<output.length(); ++j){
                if(output[j]=='1' && j<200 && i<200){
                    fastMarching.m_grid.setObstacle(j,i);
                    obstacles.push_back(fastMarching.m_grid.grid[j][i]);
                }
            }
            cout<<output<<"\n";
            ++i;
        }
    }
    myReadFile.close();*/

	it=0;
    cout<<"HOLA"<<endl;
	glutInit(&argc, argv);                 // Initialize GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    screenHeight = 900;
    glutInitWindowSize(screenHeight,screenHeight);
    glutCreateWindow("Grid");
    glutMouseFunc(mouseClicks);
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);

    glutMotionFunc(mouse_motion);
    glutPassiveMotionFunc(mouse_motion);

    glutTimerFunc(0,timer_callback,0);
    init();
    glutMainLoop();

	return 0;
}
/*
int main (){

	cGrid grid(5,5);
	cFastMarching fastMarching(10, 10, 2, 2); //cambiar grid por w,h o constructor vacio
	//fastMarching = cFastMarching(grid, 2, 2);
	fastMarching.m_grid.print();
	fastMarching.genPath(6,6);

	return 0;
}*/

