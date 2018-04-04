#include <iostream>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;

int main(){
	int k;
	cout<<"Dimension de la matriz (n*n), ingrese n: "; cin>>k;
	cout<<endl;

	 double times = 0;
    int tests = 10;
    for (int i = 0; i < tests ; i++) {

                high_resolution_clock::time_point t1 = high_resolution_clock::now();


	int ** A;	int ** B;	int ** C;

	A = new int* [k];
	for(int i = 0; i < k; i++) {A[i] = new int[k];}
	B = new int* [k];
	for(int i = 0; i < k; i++) {B[i] = new int[k];}
	C = new int* [k];
	for(int i = 0; i < k; i++) {C[i] = new int[k];}

	for(int i=0; i<k; ++i){
        for(int j=0; j<k; ++j){
			A[i][j] = rand() % 10;
			B[i][j] = rand() % 10;
			C[i][j]= 0;
		}
	}

	for(int i=0; i<k; ++i)
		for(int j=0; j<k; ++j)
			for(int z=0; z<k; ++z)
				C[i][j] += A[i][z] * B[z][j];

	/*cout<<"Matriz A: "<<endl;
	for(int i=0; i<k; ++i){
		for(int j=0; j<k; ++j){
			cout<<A[i][j]<<" ";
        }
		cout<<endl;
    }
	cout<<"Matriz B: "<<endl;
	for(int i=0; i<k; ++i){
		for(int j=0; j<k; ++j){
			cout<<B[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"Matriz C: "<<endl;
	for(int i=0; i<k; ++i){
		for(int j=0; j<k; ++j){
			cout<<C[i][j]<<" ";
		}
		cout<<endl;
	}*/


	 high_resolution_clock::time_point t2 = high_resolution_clock::now();
                 auto res = duration_cast<microseconds>( t2 - t1 ).count();
                 times += res;
            }

            long long  ttime = times/tests;
            cout << "tiempo:" << ttime/1000.0<< " milliseconds con " <<tests<<" tests \n" ;
    return 0;
}
