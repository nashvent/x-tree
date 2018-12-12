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

    int dim=2;
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
    vector<float>vbusq={3,5};
    vData knn=xt.searchKNN(vbusq,4);
    printVData(knn);
/*    ifstream file("YearPredictionMSD.txt");
    string str;
    int cont=0;
    vData vrd;
    while (std::getline(file, str))
    {
        vector<string>temp;
        string strtemp=str;
        Data rnd;
        temp=splitString(strtemp);
        for(size_t i=0;i<91;i++){
            float xtemp = ::atof(temp[18].c_str());
            rnd.push_back(xtemp);
        }
        vrd.push_back(rnd);
    }   
    cout<<"Filas: "<<vrd.size()<<endl;
    */
       
     
    return 0; 



    

}