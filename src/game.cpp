#include "pong.h"

#include <GL/gl.h>

#include <string>
#include <random>
#include <algorithm>
#include <sstream>
#include <ctime>

#include "game.h"
#include "PaddleBehavior.h"
#include "image.h"

using std::stringstream;
using std::string;

std::mt19937 rd;
std::mt19937 randomgen(rd());
std::uniform_real_distribution<float> distr(-.005,.005);

GLuint backgroundTexture;
GLuint numberTextures[10];

object leftPaddle = {{0.05,0.5}, {0.01, 0.1}, {0, 0}, {0., 1., 0.}, 0};
object rightPaddle = {{0.95,0.5}, {0.01, 0.1}, {0, 0}, {1., 0., 0.}, 0};
object ball = {{0.5, 0.5}, {0.01, 0.01}, {0.008, 0.}, {1., 1., 1.}, 0};

std::clock_t ballDelay = std::clock();

int leftScore = 0, rightScore = 0;

void newGame() {
	leftScore = rightScore = 0;
}

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
	for(int i = 0; i < 10; i++) {
		numberTextures[i] = loadImage("resources/" + toString(i) + ".png");
	}
}

void movementLogic(bool keyboardState[256]) {
	if(std::clock() > ballDelay) {
		ball.pos.x += ball.vel.x;
		ball.pos.y += ball.vel.y;
	}
	
	leftPaddleBehavior->doMovement(keyboardState);
	
	rightPaddleBehavior->doAI(ball);
	
	object oldLeftPaddle = leftPaddle;
	leftPaddle.pos.y += leftPaddle.vel.y;
	if(!leftPaddle.inBounds()) {
		leftPaddle = oldLeftPaddle;
		if(leftPaddle.pos.y < 0.5) {
			leftPaddle.pos.y = leftPaddle.dim.y / 2.;
		} else {
			leftPaddle.pos.y = 1. - leftPaddle.dim.y / 2.;
		}
	}
	
	object oldRightPaddle = rightPaddle;
	rightPaddle.pos.y += rightPaddle.vel.y;
	if(!rightPaddle.inBounds()) {
		rightPaddle = oldRightPaddle;
		if(rightPaddle.pos.y < 0.5) {
			rightPaddle.pos.y = rightPaddle.dim.y / 2.;
		} else {
			rightPaddle.pos.y = 1. - rightPaddle.dim.y / 2.;
		}
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
		ballDelay = std::clock() + CLOCKS_PER_SEC * 3 / 4;
	}
}

int stringWidth(const string& text) {
	int size = 0;
	for_each(text.begin(), text.end(), [&size](char c) { if('0' <= c && c <= '9') { size += 20; } });
	return size;
}

void drawScore(float x, float y, string& text) {
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	float width = (float)m_viewport[2];
	float height = (float)m_viewport[3];
	for(const char& c : text) {
		if('0' <= c && c <= '9') {
			GLuint num = numberTextures[c - '0'];
			
			glBindTexture(GL_TEXTURE_2D, num);
			glBegin(GL_QUADS);
			glTexCoord2i(0, 0); glVertex2f(x, y);
			glTexCoord2i(1, 0); glVertex2f(x + 50/width, y);
			glTexCoord2i(1, 1); glVertex2f(x + 50/width, y + 50/height);
			glTexCoord2i(0, 1); glVertex2f(x, y + 50/height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			
			x += 20/width;
		}
	}
}

void displayScores(int leftScore, int rightScore) {
	string leftScoreStr = toString(leftScore), rightScoreStr = toString(rightScore);
	
	//m_viewport[2] is the x width of the viewport somehow
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	float width = (float)m_viewport[2];
	
	drawScore(0, 0, leftScoreStr);
	drawScore(1 - 20/width - (float)stringWidth(rightScoreStr)/width, 0, rightScoreStr);
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
