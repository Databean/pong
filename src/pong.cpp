#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

#include "image.h"
#include "pong.h"
#include "util.h"
#include "PaddleBehavior.h"
#include "game.h"
#include "menu.h"

using std::string;
using std::abs;
using std::stringstream;
using std::for_each;

bool object::contains(float x, float y) const {
	bool xContains = abs(x - pos.x) < dim.x/2.f;
	bool yContains = abs(y - pos.y) < dim.y/2.f;
	return xContains && yContains;
}

// doesn't work with complex intersections, but don't need those here.
bool object::intersects(const object& o) const {
	return o.contains(pos.x + dim.x/2., pos.y + dim.y/2.) ||
	o.contains(pos.x + dim.x/2., pos.y - dim.y/2.) ||
	o.contains(pos.x - dim.x/2., pos.y - dim.y/2.) ||
	o.contains(pos.x - dim.x/2., pos.y + dim.y/2.);
}

bool object::inBounds() const {
	return pos.y - dim.y/2.f > 0.f &&
		pos.y + dim.y/2.f < 1.f &&
		pos.x - dim.x/2.f > 0.f &&
		pos.x + dim.x/2.f < 1.f;
}

bool keyboardState[256];
bool keyboardSpecialState[256];



void init() {
	glDepthFunc(GL_NEVER);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	
	initGame();
	initMenu();
}

void displayObject(const object& o) {
	glColor3f(o.color.r, o.color.g, o.color.b);
	glBindTexture(GL_TEXTURE_2D, o.texture);
	glBegin(GL_QUADS); {
		glTexCoord2i(0, 0); glVertex2f(o.pos.x - o.dim.x/2.f, o.pos.y - o.dim.y/2.f);
		glTexCoord2i(1, 0); glVertex2f(o.pos.x + o.dim.x/2.f, o.pos.y - o.dim.y/2.f);
		glTexCoord2i(1, 1); glVertex2f(o.pos.x + o.dim.x/2.f, o.pos.y + o.dim.y/2.f);
		glTexCoord2i(0, 1); glVertex2f(o.pos.x - o.dim.x/2.f, o.pos.y + o.dim.y/2.f);
	} glEnd();
}

void display() {
	//Clear screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//movementLogic(keyboardState);
	//drawGame();
	
	drawMenu(keyboardState);
	
	glutSwapBuffers();
	glFlush ();
	glutPostRedisplay();
}

void reshape(int w, int h) {
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	float multMatrix[16] =
	{1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1};
	glMultMatrixf(multMatrix);
	
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
	keyboardState[key] = true;
	if(key == 27) { //Escape
		glutLeaveMainLoop();
	}
}

void keyboardUp(unsigned char key, int x, int y) {
	keyboardState[key] = false;
}

void keyboardSpecial(int key, int x, int y) {
	keyboardSpecialState[key] = true;
}

void keyboardSpecialUp(int key, int x, int y) {
	keyboardSpecialState[key] = false;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glutIgnoreKeyRepeat(true);
	
	init();
	
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(keyboardSpecial);
	glutSpecialUpFunc(keyboardSpecialUp);
	glutMainLoop();
	return 0;
}
