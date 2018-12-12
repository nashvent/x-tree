#include "xtree.h"
using namespace std;

int main(){

    int dim=2;
    XTree xt=XTree(dim,2,4);
    for(int i=0;i<6;i++){
        Data rnd=randomData(dim);
        cout<<"---------------------"<<endl;
        printData(rnd);
        cout<<"---------------------"<<endl;
        
        Nodo *tmp=new Nodo(dim,rnd);
        xt.insertR(tmp); 
        xt.print();     	
    }
    xt.print();

}