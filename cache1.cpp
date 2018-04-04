#include <iostream>
#include <chrono>
#include <utility>
#include <algorithm>
using namespace std;
using namespace std::chrono;

const long long MAX= 9999;

double A[MAX][MAX],x[MAX],y[MAX];

int main(){
for (int i = 0; i < MAX ; i++){
  x[i] = 1;
  y[i]=0;
  } 
for (int i = 0; i < MAX ; i++) 
  for (int j = 0; j < MAX ; j++) 
    A[i][j]= 2;

high_resolution_clock::time_point t1 = high_resolution_clock::now();


cout<<"FIRST\n";
for (int i = 0; i < MAX ; i++) {
  for (int j = 0; j < MAX ; j++){ 
    y[i] += A[i][j]* x[j];
    }
  }

/*
cout<<"SECOND\n";
for (int j = 0; j < MAX ; j++) {
  for (int i = 0; i < MAX ; i++){ 
    y[i] += A[i][j]* x[j];
    }
  }
		*/	
high_resolution_clock::time_point t2 = high_resolution_clock::now();
auto res = duration_cast<microseconds>( t2 - t1 ).count();
cout << "tiempo:" <<res/1000.0<< " milliseconds"; 
} 
