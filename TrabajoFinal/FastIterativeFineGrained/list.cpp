

#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>
#include <thread>
#include <math.h>

using namespace std;

struct cCell {
    //The index of the cell
	int i,j;
};

void printList(list<cCell> mNarrowBand){
    cout<<"\nCOMPLETE NBand: ";
    for(list<cCell>::iterator i = mNarrowBand.begin(); i!=mNarrowBand.end(); ++i){
        cout<<"("<<(*i).i<<","<<(*i).j<<")-";
    }
}

void printVector(vector<cCell> mNarrowBand){
    cout<<"\nNBand: ";
    for(vector<cCell>::iterator i = mNarrowBand.begin(); i!=mNarrowBand.end(); ++i){
        cout<<"("<<(*i).i<<","<<(*i).j<<")-";
    }
}

void join2Vector(vector<cCell>* v1,vector<cCell>* v2){
    v1->insert(v1->end(), v2->begin(), v2->end());
}

void joinVectors(vector<vector<cCell> >* nb, int start, int fin){
    for(int i = start ; i< fin ; i++){
     nb->at(start).insert(nb->at(start).end(), nb->at(i).begin(), nb->at(i).end());   
    
    }
}

void parallelJoin(vector<vector<cCell> >* nb,int level, int round){
    thread bucket[level];

    for(int i = 0 ;i < level; i++){
        //cout<<"join:"<<round*i<<" & "<<(i*round)+(round/2)<<endl;
        bucket[i] = thread(join2Vector, &nb->at(round*i), &nb->at((i*round)+(round/2)));
    }

    for(int i=0; i<level; ++i){
        bucket[i].join();
    }
    
}

void twoThreads(vector<vector<cCell> >* nb, int size){
    thread bucket[2];

    
    bucket[0] = thread(joinVectors, nb, 0, size/ 2);
    bucket[1] = thread(joinVectors, nb, (size/ 2)+1, nb->size());
    
    for(int i=0; i<2; ++i){
        bucket[i].join();
    }
    

    v1->insert(v1->end(), v2->begin(), v2->end());
}


void parallelJoinMaster(vector<vector<cCell> >* nb, int size){
    int cont=2;
    while(size>1){
    
        //cout<<"size: "<<size;
       // cout<<" cont: "<<cont<<endl;
        parallelJoin(nb,size/2, cont);
        cont=cont*2;
        
        if(size%2){
            size=ceil(size/2 +1);
        }else{
            size=ceil(size/2);
        }
    
    }
    
}


int main(){
        int size = 5;
     //   cout<<"size: "<<size;
		list<cCell> mNarrowBand;
        vector<vector<cCell> > nb(size);
        

    for(int i =0; i<size;i++){
        nb[i].resize(size);
        for(int j =0; j<size;j++){
            nb[i][j].i=i;
            nb[i][j].j=j;
        }
    }    
    
    for(int i =0; i<size;i++){
        printVector(nb[i]);
        cout<<endl;
    }
    

    
    
    
    clock_t begin = clock();
    twoThreads(& nb,size);
//    parallelJoinMaster(&nb,size);   
    mNarrowBand.assign(nb[0].begin(), nb[0].end());
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<" Time: "<<elapsed_secs<<endl<<endl;
    printList(mNarrowBand);
    cout<<"\nCOMPLETE size"<<mNarrowBand.size()<<endl;
    
    /*clock_t begin = clock();
    for(int i=0; i<size; ++i){
            for(int j=0; j<nb[i].size(); ++j){
                //cout<<"inserto"<<endl;
                mNarrowBand.insert(mNarrowBand.begin(), nb[i][j]);            
            }
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<" Time: "<<elapsed_secs<<endl<<endl;
    //printList(mNarrowBand);
    cout<<"\nCOMPLETE size"<<mNarrowBand.size()<<endl;
    */
}
