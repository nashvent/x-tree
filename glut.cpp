#define GLUT_DISABLE_ATEXIT_HACK
#include <bits/stdc++.h>
#include <math.h>
#include <cmath>
#include <GL/glut.h>
#include "xtree.h"
#define KEY_ESC 27

using namespace std;
/*FUnciontes importantes */
vector<string> splitString(string str){
    vector<string>vect;
    string word;
    stringstream stream(str);
    while( getline(stream, word, ',') )
        vect.push_back(word);
    return vect;
}

/*Variables globales*/
XTree *rt;
vector<Nodo*> puntos;
bool ial = false;
bool showCircle=false;
int dim;

double radio=0,calx,caly,centrox,centroy;

void adaptRawPoint(int &x,int &y)
{
	x = x-300;
	y = 300-y;
}

//dibuja un simple gizmos
void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(0,255,0);
	for(unsigned i=0;i<rt->allRectangles.size();i++){
		Data xrect=rt->allRectangles[i][0];
		Data yrect=rt->allRectangles[i][1];
		//printVData(rt->allRectangles[i]);
		glVertex2d(xrect[0],yrect[0]);
		glVertex2d(xrect[1],yrect[0]);
		
		glVertex2d(xrect[1],yrect[0]);
		glVertex2d(xrect[1],yrect[1]);
		
		glVertex2d(xrect[1],yrect[1]);
		glVertex2d(xrect[0],yrect[1]);
		
		glVertex2d(xrect[0],yrect[1]);
		glVertex2d(xrect[0],yrect[0]);
	}
    glEnd();

	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3d(255,0,0);
	for(unsigned i=0;i<rt->allPoints.size();i++){	
		Data pnt=rt->allPoints[i];
		glVertex2d(pnt[0],pnt[1]);
	}
	glEnd();

	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3d(0,255,255);
	for(unsigned i=0;i<rt->radioPoints.size();i++){	
		Data pnt=rt->radioPoints[i];
		glVertex2d(pnt[0],pnt[1]);
	}
	glEnd();


	/*glPointSize(2);
	glBegin(GL_POINTS);
	glColor3d(255,255,0);
	for(unsigned i=0;i<busqueda.size();i++)
	{	
		glVertex2d(busqueda[i].x,busqueda[i].y );
	}
	glEnd();*/
	/////////////////////////////////////////////
	/*if(Ra.size()!=0 and Ra[0].x == centrox and  Ra[0].y == centroy)
	{
		glPointSize(2);
		glBegin(GL_POINTS);
		glColor3d(0,0,225);
		for(unsigned i=0;i<Ra.size();i++)
		{	
			glVertex2d(Ra[i].x,Ra[i].y);
		}


		glEnd();
	}*/
	//////////////////////////////////////////////
	if(showCircle)
	{	
		glBegin(GL_POINTS);
		glColor3d(1.0,0.5,0.0);
		radio=60;
		for(double i=0;i<15;i=i+0.01)
		{
			calx = centrox + radio * cos(i);
			caly = centroy + radio * sin(i);
			glVertex2d(calx,caly);
		}
		glEnd();

		Data busq;
		busq.push_back(centrox);
		busq.push_back(centroy);
		rt->searchRadio(busq,radio);
		
	}
	else{
		rt->radioPoints.clear();
	}
	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3d(1.0,1.0,0.0);
	
	glEnd();
	
}

void OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		adaptRawPoint(x,y);
		Data vtmp;
		vtmp.push_back(x);
		vtmp.push_back(y);
		Nodo *tmp=new Nodo(dim,vtmp);
		cout<<"##################"<<endl;
        cout<<"insert: ";
        printData(vtmp);
        rt->insert(tmp);
		rt->getRectangles();
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		showCircle=!showCircle;
		adaptRawPoint(x,y);
		centrox=x;
		centroy=y;
		
	}
}

void OnMouseMotion(int x, int y)
{ 
	adaptRawPoint(x,y);
	centrox = x;
	centroy = y;
	
}

void idle()
{ 
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {
	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-300.0f,300.0f,-300.0f,300.0f,-1.0f,1.0f);
	displayGizmo();
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) 
{
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tama�o
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-300.0f,300.0f,-300.0f,300.0f,-1.0f,1.0f);
	//glOrtho(xmin,xmax,ymin,ymax,-1.0f,1.0f);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
	default:
		break;
	}

}

void normalizar(float &x,float a,float b,float xmax,float xmin){
	x=a+((x-xmin)*(b-a))/(xmax-xmin);
}

int main(int argc, char* argv[]){
    //srand (time(NULL));
	int m,M;
	string stm=argv[1];
	string stM=argv[2];
	cout<<"m: "<<stm<<" M: "<<stM<<endl;
	m=atoi( stm.c_str());
	M=atoi( stM.c_str());
    dim=2;
    rt=new XTree(dim,m,M); //(dimension,m,M)
    vData vrd;
	float xmax=-INFINITY,xmin=INFINITY;
	float ymax=-INFINITY,ymin=INFINITY;
/*
	for(int i=0;i<10;i++){
        Data rnd=randomData(dim);
        vrd.push_back(rnd);	
		if(rnd[0]>xmax) xmax=rnd[0];
		if(rnd[0]<xmin) xmin=rnd[0];
		if(rnd[1]>ymax) ymax=rnd[1];
		if(rnd[1]<ymin) ymin=rnd[1];
	}*/

	
	ifstream file("crime50k.csv");
    string str;
    int cont=0;
    while (std::getline(file, str))
    {
        vector<string>temp;
        string strtemp=str;
        temp=splitString(strtemp);
        float xtemp = ::atof(temp[18].c_str());
        float ytemp = ::atof(temp[19].c_str());
		if(xtemp>35 and xtemp<45 ){
			if(xmax<xtemp) xmax=xtemp;
			if(xmin>xtemp) xmin=xtemp;
			if(ymax<ytemp) ymax=ytemp;
			if(ymin>ytemp) ymin=ytemp;
			Data rnd;
			rnd.push_back(xtemp);
			rnd.push_back(ytemp);
			vrd.push_back(rnd);	
		}		
		
        cont++;
    }
	cout<<"Datos leidos:" <<vrd.size()<<" registros"<<endl;
	clock_t begin = clock();

    for(int i=0;i<vrd.size();i++){
		normalizar(vrd[i][0],-300,300,xmax,xmin);
		normalizar(vrd[i][1],-300,300,ymax,ymin);
        Nodo *tmp=new Nodo(dim,vrd[i]);
		
        rt->insert(tmp);
        //rt->print();
    }
	rt->getRectangles();
	clock_t end = clock();
  	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout<<"x-tree generado en "<<elapsed_secs<<" segundos"<<endl;
	
	
	cout<<"ChooseLEaf "<<timeChosseLeaf<<endl;
	cout<<"Adjust tree "<<timeAdjustTree<<endl;
	cout<<"Split "<<timeSplit<<endl;
	cout<<"Update Rectange "<<timeUpdateRectangle<<endl;
	cout<<"AddEntry  "<<timeAddEntry<<endl;
	cout<<"TimePickSeeds  "<<timePickSeeds<<endl;
	cout<<"TimePick Next "<<timePickNext<<endl;
	cout<<"TimeMakeRectangle  "<<timeMakeRectangle<<endl;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600,600); 
	glutInitWindowPosition(400, 80); 
	glutCreateWindow("Xtree"); 

	init_GL(); 

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);
	
    //printVData(vrd);

	glutMainLoop();
    return 0;
}