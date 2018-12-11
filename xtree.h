#include <bits/stdc++.h>

using namespace std;
typedef vector<float> Data;
typedef vector<Data> vData;


/////////////////////////
float timeAdjustTree=0,timeChosseLeaf=0,timeSplit=0,timeUpdateRectangle=0,timeAddEntry=0,timePickSeeds=0,timePickNext=0,
timeMakeRectangle=0;
/////////////////////////
float area(vData mm){
    float areaCalc=1;
    for(size_t i=0;i<mm.size();i++){
        areaCalc=areaCalc*abs(mm[i][1]-mm[i][0]);
    }
    return areaCalc;
}

float distP2P(Data P1,Data P2){
    float dist=0;
    for(size_t i=0;i<P1.size();i++){
        dist+=abs(P1[i]-P2[i]);
    }
    return dist;
}


vData makeRectangle(vData E1,vData E2,float &area){
    clock_t begin = clock(); 
    vData R;
    float areaT=1;
    int E1size=E1.size();
    for(size_t i=0;i<E1size;i++){
        float tempMax,tempMin;
        if(E1[i][0]>E2[i][0])
            tempMin=E2[i][0];
        else
            tempMin=E1[i][0];

        if(E1[i][1]<E2[i][1])
            tempMax=E2[i][1];
        else
            tempMax=E1[i][1];
        R.push_back({tempMin,tempMax});
        areaT=areaT*abs(tempMax-tempMin);
    }
    area=areaT;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    timeMakeRectangle+=elapsed_secs;   

    return R;
}

vData makeRectangleFromData(Data E1){
    vData R;
    for(size_t i=0;i<E1.size();i++){
        vector<float>tempDim(2);
        tempDim[0]=tempDim[1]=E1[i];
        R.push_back(tempDim);
    }
    return R;
}

Data randomData(int dim){
    Data rsp;
    for(size_t i=0;i<dim;i++){
        int randNum=rand()%100;
        rsp.push_back((float) randNum);
    }
    return rsp;
}

void printData(Data d){
    for(size_t i=0;i<d.size();i++){
        cout<<d[i]<<", ";
    }
    cout<<endl;
}

void printVData(vData d){
    for(int i=0;i<d.size();i++){
        cout<<"["<<d[i][0]<<","<<d[i][1]<<"] ";
    }
    cout<<endl;
}

struct Nodo{
    bool isLeaf,wasSplit,isData;
    vData tuples; //tuplas [tupla,tupla,tupla] || [x,y,z,r,...],[x,y,z,r,...],[x,y,z,r,...]
    vData I; //Limites del rectangulo I Nodo x:[min,max] y:[min,max] z:[min,max] r:[min,max]
    vector<Nodo*>child; // [Nodo1,Nodo2,Nodo3,Nodo4]
    Data rPunto;
    int dim;
    Nodo *parent;
    float areac;
    Nodo(int n_dim,bool leaf=false){ //Nodo (3,true) is Leaf && Nodo(3,false) is Data
        dim=n_dim;
        isLeaf=leaf;
        isData=!leaf;
        parent=NULL;
        areac=0;
        //I.resize(n_dim);
    }
    Nodo(int n_dim,Data dt){ 
        dim=n_dim;
        isLeaf=false;
        isData=true;
        I=makeRectangleFromData(dt);
        rPunto=dt;
        areac=0;
    }
    
    bool overlap(vData pI){
        for(size_t i=0;i<dim;i++){
            if(I[i][0]>pI[i][0] or pI[i][1]>I[i][1]){
                return false;
            }
        }
        return true;
    }

    int exist(vData pI){
        for(size_t i=0;i<child.size();i++){
            if(child[i]->I==pI){
                return i;
            }
        }
        return -1;
    }

    bool deleteChild(Nodo*H){
        for(size_t i=0;i<child.size();i++)
            if(child[i]==H){
                child.erase(child.begin()+i);
                return true;
            }   
        return false;
    }

    void addEntry(Nodo *E){
        clock_t begin = clock(); 

        int ext=exist(E->I);
        if(ext==-1){
            child.push_back(E);
            E->parent=this;
            updateRectangleI();
        }
        else{
            //cout<<"Elemento existe"<<endl;
        }
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        timeAddEntry+=elapsed_secs;   
        
    }

    bool searchData(Data T){
        if ( std::find(tuples.begin(), tuples.end(), T) != tuples.end() )
            return true;
        return false;
    }
    
    void updateRectangleI(){
        clock_t begin = clock();
        
        if(child.size()==1){
            I=child[0]->I;
            areac=0;
        }
        else{
            for(size_t i=0;i<child.size();i++){
                for(size_t j=0;j<dim;j++){
                    if(child[i]->I[j][0] < I[j][0] ){
                        I[j][0]=child[i]->I[j][0];
                    }
                    if(child[i]->I[j][1] > I[j][1] ){
                        I[j][1]=child[i]->I[j][1];
                    }
                }
                
            }
            areac=area(I);
        }
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        timeUpdateRectangle+=elapsed_secs;
    }

    void print(){   
        printVData(I);
    }

};



struct XTree{
    int M,m;
    int dim;
    Nodo *root;
    vector<vData> allRectangles;
    vector<Data> allPoints;
    vector<Data> radioPoints;
    XTree(int n_dim,int n_m,int n_M){
        M=n_M;
        m=n_m;
        root=new Nodo(n_dim,true);
        dim=n_dim;
    }

    bool search(Nodo *&p,vData pI){ //Buscar un punto o un rectangulo
        // S2 /////////
        if(p->isLeaf){
            return true;
        }
        // S1 ///////
        for(size_t x=0;x<p->child.size();x++){
            Nodo*currChild=p->child[x];
            if(currChild->overlap(pI)){
                search(currChild,pI);

            }
        }
        return false;
    }

    bool insert(Nodo *E){ 
        
        allPoints.push_back(E->rPunto);
        Nodo* L,*LL;
        LL=NULL;       
        // I1 ////////
        chooseLeaf(E->I,L);
        
        // I2 ///////
        L->addEntry(E);
        if(L->child.size()>M){
            splitNode(L,LL);            
        }
        // I3 ///////
        adjustTree(L,LL); // SI no hubo split LL==NULL
        // I4 //////
        
        if(L==root and LL!=NULL){
            Nodo* tempRoot=new Nodo(dim,false);
            tempRoot->addEntry(L);
            tempRoot->addEntry(LL);
            root=tempRoot;
        }
        return true;
    }

    void chooseLeaf(vData E,Nodo *&N){
        clock_t begin = clock();
        // CL1 ////////
        N=root;
        // CL2  /////// 
        while(!N->isLeaf){
            float tempArea=INFINITY;
            Nodo *TN;
            int childSize=N->child.size();
            for(size_t i=0;i<childSize;i++){
                float nTempArea;
                makeRectangle(E,N->child[i]->I,nTempArea);
                // CL3 /////
                nTempArea=nTempArea- N->child[i]->areac;
                if(tempArea>nTempArea){
                    TN=N->child[i];
                    tempArea=nTempArea;
                }
            }
            // CL4 /////
            N=TN;
        }
        clock_t end = clock();
  	    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        timeChosseLeaf+=elapsed_secs;
        return;
    }

    void adjustTree(Nodo* &N,Nodo*&NN){ // Expand tree
        clock_t begin = clock(); 
        // AT1 | ET1 ////////
            /* Paso AT1 en la declaracion*/
        // AT2 | ET2 ////////
        while(N!=root){
            // AT3 | ET3x ///////
            Nodo *P=N->parent;
            P->isLeaf=false;
            // AT4 | ET4 ////////
            Nodo *PP=NULL;
            if(NN!=NULL){
                P->addEntry(NN);
                if(P->child.size()>M){
                    splitNode(P,PP);
                }
            }
            P->updateRectangleI();
            // AT5 | ET5 ////////
            N=P;
            NN=PP;
            
            
        }
        root->updateRectangleI();
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        timeAdjustTree+=elapsed_secs;
        return;
    }

    //Quadratic Split
    void splitNode(Nodo* &G1,Nodo* &G2){
        clock_t begin = clock(); 
        // QS1 ////////////
        Nodo *E1,*E2;
        float Ed1,Ed2;
        vector<Nodo*>LP;
        LP=G1->child;
        bool checkLeaf=G1->isLeaf;
        pickSeeds(LP,E1,E2);
        G1->child.clear();
        G2=new Nodo(dim,checkLeaf); //Nodos como grupos G1, G2;
        G1->addEntry(E1);
        G2->addEntry(E2); 
        // QS2 //////////
        while(LP.size()>0){   
            if( (G1->child.size()+LP.size())==m ){
                G1->child.insert(G1->child.end(), LP.begin(), LP.end());
                G1->updateRectangleI();
                LP.resize(0);
            }
            else if( (G2->child.size()+LP.size())==m ){
                G2->child.insert(G2->child.end(), LP.begin(), LP.end());
                G2->updateRectangleI();
                LP.resize(0);
            }
            // QS3 ////////
            if(LP.size()>0)
                pickNext(LP,G1,G2);
        }
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        timeSplit+=elapsed_secs;
        
        return;
    }

    void pickSeeds(vector<Nodo*>&LP,Nodo* &E1,Nodo* &E2){
        clock_t begin = clock(); 
        float d=-INFINITY;
        int indxE1,indxE2;
        // PS1 //////////////
        for(int x1=0;x1<LP.size();x1++){
            for(int x2=0;x2<LP.size();x2++){
                if(x1!=x2){
                    float areaJ;
                    vData J=makeRectangle(LP[x1]->I,LP[x2]->I,areaJ);
                    float a1=LP[x1]->areac;//area(LP[x1]->I);
                    float a2=LP[x2]->areac;//area(LP[x2]->I);
                    //float td=areaJ;
                    float td=areaJ-a1-a2;//areaJ*a1*a2;
                    // PS2 /////////////////////
                    if(td>=d){
                        d=td;
                        indxE1=x1;
                        indxE2=x2;
                    }
                }
            }
        }
        E1=LP[indxE1];
        E2=LP[indxE2];
                   
        /*Remover los seleccionados del grupo*/
        if(indxE1>indxE2){
            swap(indxE1,indxE2);
        }
        LP.erase(LP.begin()+indxE2);
        LP.erase(LP.begin()+indxE1);
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        timePickSeeds+=elapsed_secs;   
    }

    void pickNext(vector<Nodo*>&LP,Nodo* &G1,Nodo* &G2){    
        clock_t begin = clock();  
        int indxE1,indxE2;
        float dG1=INFINITY,dG2=INFINITY;
        //vData G1,G2;
        float aG1=G1->areac;
        float aG2=G2->areac;
        vData ntG1,ntG2;
        // PN1 ///////
        for(size_t i=0;i<LP.size();i++){
            float areaG1,areaG2;
            vData tG1=makeRectangle(G1->I,LP[i]->I,areaG1);
            vData tG2=makeRectangle(G2->I,LP[i]->I,areaG2);
            float tdG1=areaG1-aG1;
            float tdG2=areaG2-aG2;
            if(tdG1<dG1){
                indxE1=i; //Indice en LP
                dG1=tdG1; //Diferencia agregando la nueva entrada
                ntG1=tG1; //nuevo Rectangulo ->  I
            }
            if(tdG2<dG2){
                indxE2=i;
                dG2=tdG2;
                ntG2=tG2;           
            }
            
        }
        // PN2 //////////////
        if(dG1<dG2){
            G1->addEntry(LP[indxE1]);
            LP.erase(LP.begin()+indxE1);
        }
        else{   
            G2->addEntry(LP[indxE2]);
            LP.erase(LP.begin()+indxE2);
        }
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        timePickNext+=elapsed_secs;
    }

    void print(){
        cout<<"========================================="<<endl;
        cout<<"Imprimiendo Arbol"<<endl;
        cout<<"========================================="<<endl;
        printR(root);    
    }

    void printR(Nodo *P){
        cout<<"_________";
        cout<<" Nodo ";      
        cout<<"_________"<<endl;
        P->print();
        if(P->parent==NULL)
            cout<<" - Padre null - ";
        else{
            cout<<" - Padre -";
            P->parent->print();
        }
        cout<<endl;
        if(P->isLeaf){
            cout<<"----------";
            cout<<" Hoja ";
            cout<<"----------"<<endl;
            for(int i=0;i<P->child.size();i++){
                cout<<" --> ";
                P->child[i]->print();
            }
            return;
        }
        
        for(int i=0;i<P->child.size();i++){
            printR(P->child[i]);
        }
    }
    void getRectangles(){
        allRectangles.clear();
        getRectanglesR(root);
    }

    void getRectanglesR(Nodo *P){
        for(size_t i=0;i<P->child.size();i++){
            allRectangles.push_back(P->I);
            getRectanglesR(P->child[i]);
        }
        if(P->isLeaf){
            return;
        }
    }

    float getMinimalDistance(Nodo *P, Data Punto){
        float dist=0;
        for(size_t i=0;i<Punto.size();i++){
            if(P->I[i][0]<=Punto[i] and P->I[i][1]>=Punto[i]){
                dist+=0;    
            }
            else{
                float tdMin=pow(P->I[i][0]-Punto[i],2);
                float tdMax=pow(P->I[i][1]-Punto[i],2);
                if(tdMin<tdMax)
                    dist+=tdMin;
                else
                    dist+=tdMax;
            }
        }
        return sqrt(dist);
    }

    void searchRadio(Data Punto, float radio){
        vector<Nodo*> RList=root->child;
        Data RDist;
        radioPoints.clear();
        for(size_t i=0;i<RList.size();i++){
            if(RList[i]->isLeaf){
                Nodo*LN=RList[i];
                for(int j=0;j<LN->child.size();j++){
                    Nodo*tempLN=LN->child[j];
                    Data TP=tempLN->rPunto;
                    if(distP2P(Punto,TP)<radio){
                        radioPoints.push_back(TP);
                    }
                }
            }
            else{
                float tdist=getMinimalDistance(RList[i],Punto);
                if(tdist<=radio){
                    RList.insert(RList.end(), RList[i]->child.begin(), RList[i]->child.end());
                }
            }

        }

    }
};
