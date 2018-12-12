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


float overlap(vData D1,vData D2){
    if(D1.size()== D2.size()) {
        float ovlp=0;
        int dsize=D1.size();
        for(size_t i=0;i<dsize;i++){
            Data tD1=D1[i];
            Data tD2=D2[i];
            if(tD1[0]<tD2[1] and tD2[0]<tD1[1]){
                float minMax,maxMin;
                if(tD1[1]<tD2[1]){
                    minMax=tD1[1];
                }
                else{
                    minMax=tD2[1];
                }

                if(tD1[0]>tD2[0]){
                    maxMin=tD1[0];
                }
                else{
                    maxMin=tD2[0];
                }
                ovlp+=abs(minMax-maxMin);

            }
        }
        return ovlp;
    }
    return 0;

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
    bool supernode;
    Nodo(int n_dim,bool leaf=false){ //Nodo (3,true) is Leaf && Nodo(3,false) is Data
        dim=n_dim;
        isLeaf=leaf;
        isData=!leaf;
        parent=NULL;
        areac=0;
        supernode=false;
        //I.resize(n_dim);
    }
    Nodo(int n_dim,Data dt){ 
        dim=n_dim;
        isLeaf=false;
        isData=true;
        I=makeRectangleFromData(dt);
        rPunto=dt;
        areac=0;
        supernode=false;
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

    void addEntry(Nodo *E){
        clock_t begin = clock(); 
        int ext=exist(E->I);
        if(ext==-1){
            child.push_back(E);
            E->parent=this;
            //updateRectangleI();
            if(child.size()==1){
                I=E->I;
            }else{
                addMBR(E);
            }
            
        }
        else{
            //cout<<"Elemento existe"<<endl;
        }
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        timeAddEntry+=elapsed_secs;   
        
    }

    void addMBR(Nodo *E){
        I=makeRectangle(I,E->I,areac);
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

    Nodo *chooseSubTree(Nodo *E){
        if(this->isLeaf){
            return this;
        }
        else{
            vData EI=E->I;
            float tempArea=INFINITY;
            Nodo *TN;
            int childSize=this->child.size();
            for(size_t i=0;i<childSize;i++){
                float nTempArea;
                makeRectangle(EI,this->child[i]->I,nTempArea);
                // CL3 /////
                nTempArea=nTempArea- this->child[i]->areac;
                if(tempArea>nTempArea){
                    TN=this->child[i];
                    tempArea=nTempArea;
                }
            }
            return TN;   
        }
    }

};



struct XTree{
    int M,m;
    int dim;
    float max_overlap;
    Nodo *root;
    vector<vData> allRectangles;
    vector<Data> allPoints;
    vector<Data> radioPoints;
    XTree(int n_dim,int n_m,int n_M){
        M=n_M;
        m=n_m;
        root=new Nodo(n_dim,true);
        dim=n_dim;
        max_overlap=dim*10;
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

    //1:split, 2:supernodo 3:nada
    int operarNodo(Nodo* current){
        Nodo*new_son1,*new_son2;
        if(current->child.size()>M){
            if(split(current,new_son1,new_son2)){
                Nodo *padre;
                if(current==root){
                    padre=new Nodo(dim);
                    root=padre;
                }
                else{
                    padre=current->parent;
                }
                new_son1->isLeaf=current->isLeaf;
                new_son2->isLeaf=current->isLeaf;
                padre->addEntry(new_son1);
                padre->addEntry(new_son2);
                return 1;
            }
            else{
                current->supernode=true;
                return 2;
            }
        }
        return 3;

    }
    
    void insertR(Nodo *entry){
        Nodo *p=root;
        insert(entry,p);
    }

    int insert(Nodo*entry,Nodo*&current){
        if(current->isLeaf){
            current->addEntry(entry);
            return operarNodo(current);
        }
        Nodo*follow,*new_son;
        int return_value;
        
        follow=current->chooseSubTree(entry);
        return_value=insert(entry,follow);
        if(return_value==1){
            return operarNodo(current);
        }
        else if(return_value==2){
            //cout<<"Cree un super nodo"<<endl;
        }
        return 3;
    } 
    
    /*
    int insertR(Nodo* entry,Nodo* father){
        int return_value;
        Nodo *follow,*new_son;
        if(father->isLeaf){
            follow=father;
            //father=new Nodo(dim);

            father=NULL;
            follow->addEntry(entry);
            if(follow->child.size()>M and follow->supernode==false)
                return_value=2;
            else
                return_value=3;
        }
        else{
            follow=father->chooseSubTree(entry);
            return_value=insertR(entry,follow);
        }
        if(return_value==2){ //SPLIT
            if(split(follow,new_son)==true){
                if(father==NULL){
                    father=new Nodo(dim);
                    root=father;
                    father->addEntry(follow);
                }
                father->addEntry(new_son);
                if(father->child.size()>M)
                    return 2;
                return 3;
            }   
            else{
                follow->supernode=true;
                return 1;
            }            
        } 
        else if(return_value==1){
            cout<<"Aparecio un supernodo"<<endl;
        }

        return 3;
    }
    */


    bool split(Nodo*curr,Nodo*&te1,Nodo* &te2){
        splitNode2(curr->child,te1,te2);
        float ovlp=overlap(te1->I,te2->I);
        if(ovlp<max_overlap){
            return true;
        }
        return false;
    }

    void splitNode2(vector<Nodo*>LP,Nodo* &G1,Nodo* &G2){
        G1=new Nodo(dim);
        G2=new Nodo(dim);
        int lpsize=LP.size();
        for(int i=0;i<lpsize;i++){
            if(i<lpsize/2){
                G1->addEntry(LP[i]);
            }
            else{
                G2->addEntry(LP[i]);
            }
        }

    }

    void splitNode(vector<Nodo*>LP,Nodo* &G1,Nodo* &G2){
        clock_t begin = clock(); 
        Nodo *E1,*E2;
        float Ed1,Ed2;
        pickSeeds(LP,E1,E2);       
        G1=new Nodo(dim,false);
        G2=new Nodo(dim,false); //Nodos como grupos G1, G2;
        G1->addEntry(E1);
        G2->addEntry(E2);
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

            if(LP.size()>0){
                pickNext(LP,G1,G2);
            }
                
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
                   
        // Remover los seleccionados del grupo ////
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

    void searchKNN(Data punto){
        vector<Nodo*>Pendientes;
        vector<Nodo*>Puntos;
        Nodo*Ntemp=root;
        int stemp=Ntemp->child.size();
        float mdist=INFINITY;
        for(int i=0;i<stemp;i++){
            float tmdist=
        }

    }

};
