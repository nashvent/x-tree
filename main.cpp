#include "xtree.h"
using namespace std;

int main(){

    int dim=30;
    XTree xt=XTree(dim,2,4);
    clock_t begin = clock();  
    for(int i=0;i<1000;i++){     
        Data rnd=randomData(dim);
        Nodo *tmp=new Nodo(dim,rnd);
        xt.insertR(tmp);    
        if(i%100==0){
            cout<<i<<endl;
        }
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"tiempo: "<<elapsed_secs<<endl;
}