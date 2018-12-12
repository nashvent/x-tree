#include "xtree.h"
using namespace std;



vector<string> splitString(string str){
    vector<string>vect;
    string word;
    stringstream stream(str);
    while( getline(stream, word, ',') )
        vect.push_back(word);
    return vect;
}


int main(){

    /*int dim=2;
    XTree xt=XTree(dim,2,4);
    // Prueba de insercion //
    
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
    /// Prueba de busqueda ////
    vector<float>vbusq={3,5};
    vData knn=xt.searchKNN(vbusq,4);
    printVData(knn);
    */
    int m=500,M=1000;
    int dim=91;
    XTree xt=XTree(dim,m,M);
    ifstream file("YearPredictionMSD.txt");
    string str;
    int cont=0;
    vData vrd;
    
    while (std::getline(file, str))
    {
        vector<string>temp;
        string strtemp=str;
        Data rnd;
        rnd.reserve(dim);
        temp=splitString(strtemp);
        for(size_t i=0;i<dim;i++){
            float xtemp = ::atof(temp[i].c_str());
            rnd.push_back(xtemp);
        }
        vrd.push_back(rnd);
        cont++;
    }   

    cout<<"Inicia insercion"<<endl;
    cout<<"m: "<<m<<" M:"<<M<<endl;
    clock_t begin = clock();
    for(size_t t=0;t<vrd.size();t++){
        Nodo *tmp=new Nodo(dim,vrd[t]);
        xt.insertR(tmp);    
        if(t%100000==0)
            cout<<t<<" elementos insertados"<<endl;
    }

    cout<<vrd.size()<<" elementos insertados"<<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"tiempo: "<<elapsed_secs<<" segundos"<<endl;
    
    begin = clock();
    
    vData knn=xt.searchKNN(vrd[0],4);
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"Tiempo KNN: "<<elapsed_secs<<" segundos"<<endl;
    //printVData(knn);
    
    return 0; 
}