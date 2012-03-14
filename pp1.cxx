/*********************************************************************************/
/* Programming Project 2                                                         */
/*********************************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "pp1.h"
#include "pp1_ui.h"
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

#include <FL/Fl_Widget.H>

#include "Datafile.h"
#include "Matrix.h"
#include "Path.h"
#include "Tour.h"
#include "Spline.h"
#include "Triangulator.h"

#define VIEW_X_DEFAULT     1.0
#define VIEW_Y_DEFAULT     1.0
#define VIEW_Z_DEFAULT     1.0

#define D_MIN 0.0
#define D_MAX 100.0
#define D_DEFAULT 0.0

using namespace std;

Fl_Double_Window *ui;

GLdouble viewX, viewY, viewZ;             /* view point */
GLdouble centerX, centerY, centerZ;             /* view point */

GLUquadricObj *theQuadric;
GLUnurbsObj *theNurb;

//Datafile *datafile;
Triangulator *datafile;
Tour *tour;

void dCallback(Fl_Value_Slider *slider, long)
{
    float d = slider->value();
    tour->d(d);
    safetyValue->value(tour->path->distance());
}


void DefineLight();
void DefineMaterial();
void DefineViewPointMain();

void DrawScene(){
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    glVertex3f(centerX, centerY, centerZ);
    glEnd();
    glPointSize(0);
    DefineLight();
    DefineMaterial();
    datafile->draw();
    tour->draw();
}

bool paused = true;

void PauseProgram(Fl_Button*, void *)
{
    cout << "p" << endl;
    paused = !paused;
}

void AddPoint(Fl_Button*, void*)
{
}

void SixtyHzCallback(void *)
{
    Fl::repeat_timeout(1.0/60, SixtyHzCallback);
    if(paused)
        return;
    static GLfloat t;
    if(t>tour->length())
    {
        t = 0;
        paused = true;
    }
    t+=.01;
    POIbrowser->select(int(t+.5)/3+1);

    Point3<GLfloat> location = tour->evaluate(t);
    Point3<GLfloat> lookAt = tour->evaluate(t+.5);
    viewX = location[0];
    viewY = location[1];
    viewZ = location[2];
    centerX = lookAt[0];
    centerY = lookAt[1];
    centerZ = lookAt[2];
    
    canvas->flush();
    canvas2->flush();
}

// Callback for the button that controls the exit of the program
void QuitProgram(Fl_Button *ob, long data){
	exit(0);
}

// Define the default values of the interface
void InitInterfaceDefaults(void){
    
    DSlider->bounds(D_MIN, D_MAX);
    DSlider->value(D_DEFAULT);
  
	viewX      = VIEW_X_DEFAULT ;
	viewY      = VIEW_Y_DEFAULT ;
	viewZ      = VIEW_Z_DEFAULT ;

}

void MyInit(void){
	theQuadric = gluNewQuadric();
    datafile = new Triangulator("hw4.heights");
    tour = new Tour("hw4.tour", datafile);
    for(vector<Point3<GLfloat> >::iterator iter = tour->path->points.begin(); iter!=tour->path->points.end(); iter++)
    {
        stringstream ss;
        ss << *iter;
        POIbrowser->add(ss.str().c_str(), 0);
    }
    POIbrowser->select(1);

    segmentsValue->value(tour->path->duration());
    lengthValue->value(tour->path->length());
    curvatureValue->value(tour->path->curvature());
    safetyValue->value(tour->path->distance());
    
    Fl::add_timeout(1.0/60, SixtyHzCallback);
}

void test();
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    test();
	ui = create_the_forms();
	Fl::visual(FL_DOUBLE|FL_INDEX);
	ui->label("Torus");
	ui->show(argc, argv);

	InitInterfaceDefaults();

	MyInit();

	Fl::run();
	return 0;
}

///////////////////////////////////////////////////////////////////////
/////////// You should not need to edit anything below here ///////////
///////////////////////////////////////////////////////////////////////

// Define viewpoint of main window
void DefineViewPointMain(){
	glLoadIdentity();
    gluLookAt(
        viewX, viewY, viewZ,
        centerX, centerY, centerZ,
        0.0, 0.0, 1.0);
}

// Define viewpoint of the secondary window
void DefineViewPointSecondary(){
	glLoadIdentity();
	gluLookAt(
		0.0, 0.0, 3000.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}

// Create the axes
void DrawAxes(void){
	glDisable(GL_LIGHTING);
	glLineWidth(3.0);
	glBegin (GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f( 0.0, 0.0, 0.0 );   glVertex3f( 0.0, 0.0, 10.0 );
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f( 0.0, 0.0, 0.0 );   glVertex3f( 0.0, 10.0, 0.0 );
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f( 0.0, 0.0, 0.0 );   glVertex3f( 10.0, 0.0, 0.0 );
	glEnd();
	glEnable(GL_LIGHTING);
}

// Create the viewpoint lines and torus
void DrawViewpoint(void){
	glDisable(GL_LIGHTING);
	glLineWidth(3.0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin (GL_LINES);
	glVertex3f( 0.0, 0.0, 0.0 ); glVertex3f( viewX, viewY, viewZ );
	glEnd();
	glTranslatef(viewX, viewY, viewZ);
	glEnable(GL_LIGHTING);
	gluSphere(theQuadric, 0.5, 10, 10);
}

// Define the light sources
void DefineLight(void){
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient); 
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); 
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient); 
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse); 

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
    //glShadeModel(GL_FLAT);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
}

// Define the material
void DefineMaterial(void){
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
}

MainCanvas::MainCanvas(int X, int Y, int W, int H)
 : Fl_Gl_Window(X, Y, W, H, "")
{
}

int MainCanvas::handle(int event){
	return Fl_Gl_Window::handle(event);
}

void MainCanvas::draw(){
	/* Draw in the main window */
    DefineViewPointMain();
	if (!valid()) {
	  glViewport(0,0, (GLint)w(), (GLint)h());
	  glMatrixMode( GL_PROJECTION );
	  glLoadIdentity();
	  gluPerspective (75.0, (GLdouble)w()/(GLdouble)h(), 0.1, 200000.0);
	  glMatrixMode( GL_MODELVIEW );
	  DefineLight();
	  DefineMaterial();
	}
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DefineViewPointMain();

	DrawScene();
}

CameraPositionCanvas::CameraPositionCanvas(int X, int Y, int W, int H)
 : Fl_Gl_Window(X, Y, W, H, "")
{
}

int CameraPositionCanvas::handle(int event){
	return Fl_Gl_Window::handle(event);
}
void CameraPositionCanvas::draw(){
    DefineViewPointSecondary();
	if (!valid()) {
		glViewport(0,0, (GLint)w(), (GLint)h());
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective (75.0, (GLdouble)w()/(GLdouble)h(), 0.1, 40000.0);
		glMatrixMode( GL_MODELVIEW );
		DefineLight();
		DefineMaterial();
	}
	/* Draw in the secondary window */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DefineViewPointSecondary();

	DrawScene();
	
	DrawAxes();
	DrawViewpoint();
}

void test()
{
    /*
    Matrix<GLfloat,4,6> mat1;
    Matrix<GLint,6,2> mat2;
    Matrix<GLint,4,2> mat3 = mat1*(mat2);*/
}
