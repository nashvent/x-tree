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

    int dim=91;
    XTree xt=XTree(dim,50,100);
    ifstream file("YearPredictionMSD.txt");
    string str;
    int cont=0;
    vData vrd;
    clock_t begin = clock();
    
    while (std::getline(file, str))
    {
        vector<string>temp;
        string strtemp=str;
        Data rnd;
        temp=splitString(strtemp);
        for(size_t i=0;i<dim;i++){
            float xtemp = ::atof(temp[18].c_str());
            rnd.push_back(xtemp);
        }
        Nodo *tmp=new Nodo(dim,rnd);
        xt.insertR(tmp);    
        
        vrd.push_back(rnd);
        cont++;
    }   
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"tiempo: "<<elapsed_secs<<endl;
    cout<<"Tiempo addentry "<<timeAddEntry<<endl;
    cout<<"Tiempo makerectange "<<timeMakeRectangle<<endl;
    cout<<"Tiempo operarNodo "<<timeOperarNodo<<endl;
    cout<<"Tiempo overlap "<<timeOverlap<<endl;
    vData knn=xt.searchKNN(vrd[0],4);
    cout<<"Pasee"<<endl;
    //xt.print();
    printVData(knn);
    return 0; 
}