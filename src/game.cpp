#include "pong.h"

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <string>
#include <random>
#include <algorithm>
#include <sstream>

#include "game.h"
#include "PaddleBehavior.h"
#include "image.h"

using std::stringstream;
using std::string;

std::random_device rd;
std::mt19937 randomgen(rd());
std::uniform_real_distribution<float> distr(-.005,.005);

GLuint backgroundTexture;

object leftPaddle = {{0.05,0.5}, {0.01, 0.1}, {0, 0}, {0., 1., 0.}, 0};
object rightPaddle = {{0.95,0.5}, {0.01, 0.1}, {0, 0}, {1., 0., 0.}, 0};
object ball = {{0.5, 0.6}, {0.01, 0.01}, {0.008, 0.}, {1., 1., 1.}, 0};

int leftScore = 0, rightScore = 0;

template<class T>
string toString(const T& t) {
	stringstream s;
	s << t;
	return s.str();
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

PaddleBehavior* leftPaddleBehavior = new JumpPaddle(leftPaddle, 0.01, 0.06);
PaddleBehavior* rightPaddleBehavior = new JumpPaddle(rightPaddle, 0.01, 0.06);

void initGame() {
	backgroundTexture = loadImage("resources/NeonVariant.png");
}

void movementLogic(bool keyboardState[256]) {
	ball.pos.x += ball.vel.x;
	ball.pos.y += ball.vel.y;
	
	leftPaddleBehavior->doMovement(keyboardState);
	
	rightPaddleBehavior->doAI(ball);
	
	object oldLeftPaddle = leftPaddle;
	leftPaddle.pos.y += leftPaddle.vel.y;
	if(!leftPaddle.inBounds()) {
		leftPaddle = oldLeftPaddle;
	}
	
	object oldRightPaddle = rightPaddle;
	rightPaddle.pos.y += rightPaddle.vel.y;
	if(!rightPaddle.inBounds()) {
		rightPaddle = oldRightPaddle;
	}
	
	
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
}

void drawGame() {
	
	
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(0, 0);
	glTexCoord2i(0, 1); glVertex2i(0, 1);
	glTexCoord2i(1, 1); glVertex2i(1, 1);
	glTexCoord2i(1, 0); glVertex2i(1, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	displayObject(leftPaddle);
	displayObject(rightPaddle);
	displayObject(ball);
	displayScores(leftScore, rightScore);
}
