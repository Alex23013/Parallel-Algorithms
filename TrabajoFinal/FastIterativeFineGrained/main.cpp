#include <cstdlib>
#include <iostream>
#include <random>
#include <time.h>
#include <fstream>
#include <GL/gl.h>
#include <GL/glut.h>

//#include "grilla_FI.hpp"
#include "../FastMarching/grilla_FM.hpp"
#include "fast_iterative.hpp"

#define COLUMNS 10
#define ROWS 10
#define FPS 60

/*
g++ -std=c++11 main.cpp fast_iterative.cpp -o demo1 -lglut -lGL -lpthread

*/
using namespace std;

int it, inix, iniy, goalx, goaly, countClicks, screenHeight;
bool iniSeted, goalSeted, clickRightDown, pathFound;

vector<cCell> visited, path, obstacles;

cFastIterative fastIterative;

void init(){

    glClearColor(0.0,0.0,0.0,1.0);
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

    for(int i=0; i<fastIterative.path.size(); ++i){
        glColor3f(1.0,0.0,0.0);
        fillCell(fastIterative.path[i]->i,fastIterative.path[i]->j);
    }
    
}

void display_callback(){

    int x,y;
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    
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
        ++countClicks;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        
        //OJO:
        clock_t begin = clock();
        pathFound = fastIterative.FI(goalx,goaly, inix, iniy, -1);//20);
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout<<"Time: "<<elapsed_secs<<endl<<endl;
        //fastIterative.m_grid.print();
        if(pathFound) fastIterative.genPath(inix,iniy);
        visited = fastIterative.visited;
        cout<<"Cells: "<<visited.size()<<endl<<endl;
        //path = fastIterative.path;
    }
}

void mouse_motion(int x, int y)
{
    if(countClicks>1 && clickRightDown){
        if((y/(screenHeight/COLUMNS)!=goalx || x/(screenHeight/COLUMNS)!=goaly) && (y/(screenHeight/COLUMNS)!=inix || x/(screenHeight/COLUMNS)!=iniy)) 
            fastIterative.m_grid.setObstacle(y/(screenHeight/COLUMNS),x/(screenHeight/COLUMNS));
        for(int i=0; i<fastIterative.m_grid.grid.size(); ++i){
            for(int j=0; j<fastIterative.m_grid.grid[i].size(); ++j){
                if(fastIterative.m_grid.grid[i][j].obstacle)
                    obstacles.push_back(fastIterative.m_grid.grid[i][j]);
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
    
    fastIterative = cFastIterative (COLUMNS, ROWS);
	
    /*//Load obstacles of a maze-like map
    
    ifstream myReadFile;
    myReadFile.open("test.txt");
    string  output;
    if (myReadFile.is_open()) {
        int i=0;
        while (!myReadFile.eof()) {

            myReadFile >> output;
            for(int j=0; j<output.length(); ++j){
                if(output[j]=='1' && j<200 && i<200){
                    fastIterative.m_grid.setObstacle(j,i);
                    obstacles.push_back(fastIterative.m_grid.grid[j][i]);
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
    glutCreateWindow("FIM Test");
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
