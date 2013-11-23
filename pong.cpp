#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <random>

#include "image.h"

using std::string;
using std::abs;
using std::stringstream;
using std::for_each;

template<class T>
string toString(const T& t) {
	stringstream s;
	s << t;
	return s.str();
}

bool keyboardState[256];
bool keyboardSpecialState[256];
std::random_device rd;
std::mt19937 randomgen(rd());
std::uniform_real_distribution<float> distr(-.005,.005);

struct object {
	struct {
		float x, y;
	} pos;
	struct {
		float x, y;
	} dim;
	struct {
		float x, y;
	} vel;
	struct {
		float r, g, b;
	} color;
	
	bool contains(float x, float y) const {
		bool xContains = abs(x - pos.x) < dim.x/2.f;
		bool yContains = abs(y - pos.y) < dim.y/2.f;
		return xContains && yContains;
	}
	
	// doesn't work with complex intersections, but don't need those here.
	bool intersects(const object& o) const {
		return o.contains(pos.x + dim.x/2., pos.y + dim.y/2.) ||
			o.contains(pos.x + dim.x/2., pos.y - dim.y/2.) ||
			o.contains(pos.x - dim.x/2., pos.y - dim.y/2.) ||
			o.contains(pos.x - dim.x/2., pos.y + dim.y/2.);
	}
};

object leftPaddle = {{0.05,0.5}, {0.01, 0.1}, {0, 0}, {0., 1., 0.}};
object rightPaddle = {{0.95,0.5}, {0.01, 0.1}, {0, 0}, {1., 0., 0.}};
object ball = {{0.5, 0.6}, {0.01, 0.01}, {0.008, 0.}, {1., 1., 1.}};

int leftScore = 0, rightScore = 0;

GLuint backgroundTexture;

void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	
	backgroundTexture = loadImage("Neon-Variant.png");
}

float signum(float x) {
	if(x < 0) {
		return -1;
	} else if(x == 0) {
		return 0;
	} else {
		return 1;
	}
}

float limit(float x, float min, float max) {
	if(x < min) {
		return min;
	} else if(x > max) {
		return max;
	} else {
		return x;
	}
}

float dash = 0;

void movementLogic() {
	ball.pos.x += ball.vel.x;
	ball.pos.y += ball.vel.y;
	
	float leftSpeed = 0.01;
	float dashSpeed = 0.06;
	
	if(dash > .2) {
		
	} else if(dash > 0) {
		leftPaddle.vel.y = 0;
	} else {
		leftPaddle.vel.y = 0;
		if(keyboardState['s'] && leftPaddle.pos.y - leftPaddle.dim.y/2.f > 0.f) {
			leftPaddle.vel.y = -leftSpeed;
		}
		else if(keyboardState['w'] && leftPaddle.pos.y + leftPaddle.dim.y/2.f < 1.f) {
			leftPaddle.vel.y = leftSpeed;
		} else {
			leftPaddle.vel.y = 0;
		}
		
		if(keyboardState[' ']) {
			int dir = signum(leftPaddle.vel.y);
			dash = 0.26;
			leftPaddle.vel.y = dashSpeed * dir;
		}
	}
	
	dash -= 0.01;
	
	if(rightPaddle.pos.y > ball.pos.y) {
		rightPaddle.vel.y = -0.014;
	}
	else if(rightPaddle.pos.y < ball.pos.y) {
		float aiSpeed = 0.014;
		if(abs(rightPaddle.pos.y - ball.pos.y) < aiSpeed) {
			rightPaddle.vel.y = ball.pos.y - rightPaddle.pos.y;
		} else {
			rightPaddle.vel.y = aiSpeed;
		}
	} else {
		rightPaddle.vel.y = 0;
	}
	
	leftPaddle.pos.y += leftPaddle.vel.y;
	rightPaddle.pos.y += rightPaddle.vel.y;
	
	if(ball.pos.y >= 1 || ball.pos.y <= 0) {
		ball.vel.y *= -1.f;
	}
	
	if(ball.intersects(leftPaddle) || ball.intersects(rightPaddle)) {
		ball.vel.x = ball.intersects(leftPaddle) ? 0.01 : -0.01;
		float vel = (ball.intersects(leftPaddle) ? leftPaddle : rightPaddle).vel.y;
		float ydiff = (ball.intersects(leftPaddle) ? leftPaddle : rightPaddle).pos.y - ball.pos.y;
		ball.vel.y += 0.25 * vel - 0.07 * ydiff;
	}
	
	bool restart = false;
	if(ball.pos.x < -.2) {
		rightScore++;
		restart = true;
	}
	if(ball.pos.x > 1.2) {
		leftScore++;
		restart = true;
	}
	if(restart) {
		ball.pos.x = 0.5;
		ball.pos.y = 0.5;
		ball.vel.y = distr(randomgen);
	}
}

void displayObject(const object& o) {
	glColor3f(o.color.r, o.color.g, o.color.b);
	glBegin(GL_QUADS); {
		glVertex2f(o.pos.x - o.dim.x/2.f, o.pos.y - o.dim.y/2.f);
		glVertex2f(o.pos.x + o.dim.x/2.f, o.pos.y - o.dim.y/2.f);
		glVertex2f(o.pos.x + o.dim.x/2.f, o.pos.y + o.dim.y/2.f);
		glVertex2f(o.pos.x - o.dim.x/2.f, o.pos.y + o.dim.y/2.f);
	} glEnd();
}

/**
 * Uses glutBitmapWidth to find the width (in pixels) of some bitmap font.
 */
int stringWidth(void* font, const string& text) {
	int size = 0;
	for_each(text.begin(), text.end(), [font, &size](char c) { size += glutBitmapWidth(font, c); });
	return size;
}

void displayScores(int leftScore, int rightScore) {
	string leftScoreStr = toString(leftScore), rightScoreStr = toString(rightScore);
	auto font = GLUT_BITMAP_HELVETICA_18;
	
	glRasterPos2f(0, 0);
	glutBitmapString(font, reinterpret_cast<const unsigned char*>(leftScoreStr.c_str()));
	
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	
	//m_viewport[2] is the x width of the viewport somehow
	glRasterPos2f(1 - (float)stringWidth(font, rightScoreStr)/(float)m_viewport[2], 0);
	glutBitmapString(font, reinterpret_cast<const unsigned char*>(rightScoreStr.c_str()));
	
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(0, 0);
	glTexCoord2i(0, 1); glVertex2i(0, 1);
	glTexCoord2i(1, 1); glVertex2i(1, 1);
	glTexCoord2i(1, 0); glVertex2i(1, 0);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void display() {
	//Clear screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	displayObject(leftPaddle);
	displayObject(rightPaddle);
	displayObject(ball);
	displayScores(leftScore, rightScore);
	
	movementLogic();
	
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
