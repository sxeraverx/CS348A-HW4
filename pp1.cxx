/*********************************************************************************/
/* Programming Project 2: Sample program to visualize a torus in OpenGL         */
/*********************************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include "pp1.h"
#include "pp1_ui.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "Datafile.h"
#include "Matrix.h"
#include "Tour.h"
#include "Spline.h"
#include "Triangulator.h"

#define VIEW_X_DEFAULT     10.0
#define VIEW_Y_DEFAULT     10.0
#define VIEW_Z_DEFAULT     10.0

#define M_MIN -10.0
#define M_MAX 10.0
#define M_DEFAULT 0.0

Fl_Double_Window *ui;

GLdouble viewX, viewY, viewZ;             /* view point */

GLUquadricObj *theQuadric;
GLUnurbsObj *theNurb;

//Datafile *datafile;
Triangulator *datafile;
Tour *tour;

void mCallback(Fl_Value_Slider *slider, long)
{
    float m = slider->value();
    std::cout << m << std::endl;
    tour->m(m);
}


void DefineLight();
void DefineMaterial();
void DefineViewPointMain();

void DrawScene(){
    DefineLight();
    DefineMaterial();
    datafile->draw();
    tour->draw();
}

void SixtyHzCallback(void *)
{
    Fl::repeat_timeout(1.0/60, SixtyHzCallback);
    double r = sqrt(pow(viewX, 2) + pow(viewY, 2) + pow(viewZ, 2));
    double theta = acos(viewZ/r);
    double phi = atan(viewY/viewX);
    if(phi<0 && viewY > 0)
        phi += M_PI;
    if(phi>0 && viewY < 0)
        phi -= M_PI;
    phi += M_PI / 240;
    //theta += (phi>0?M_PI:-M_PI)/400;
    viewX = r * cos(phi) * sin(theta);
    viewY = r * sin(phi) * sin(theta);
    viewZ = r * cos(theta);
    canvas->flush();
}

// Callback for the button that controls the exit of the program
void QuitProgram(Fl_Button *ob, long data){
	exit(0);
}

// Define the default values of the interface
void InitInterfaceDefaults(void){

    MSlider->bounds(M_MIN, M_MAX);
    MSlider->value(M_DEFAULT);
  
	viewX      = VIEW_X_DEFAULT ;
	viewY      = VIEW_Y_DEFAULT ;
	viewZ      = VIEW_Z_DEFAULT ;

}

void MyInit(void){
	theQuadric = gluNewQuadric();
    datafile = new Triangulator("hw4.heights");
    tour = new Tour("hw4.tour");
    Fl::add_timeout(1.0/60, SixtyHzCallback);
}

void test();
int main(int argc, char *argv[]){
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
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0);
}

// Define viewpoint of the secondary window
void DefineViewPointSecondary(){
	glLoadIdentity();
	gluLookAt(
		6.0, 10.0, 13.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0);
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
    glShadeModel(GL_SMOOTH);
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
	  gluPerspective (75.0, (GLdouble)w()/(GLdouble)h(), 0.1, 200.0);
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
		gluPerspective (75.0, (GLdouble)w()/(GLdouble)h(), 0.1, 20.0);
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
