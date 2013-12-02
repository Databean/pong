#include "menu.h"

#include "image.h"

GLuint menuTexture;
GLuint playTexture;
GLuint playSelectedTexture;
GLuint modeTexture;
GLuint modeSelectedTexture;
GLuint optionTexture;
GLuint optionSelectedTexture;

void initMenu() {
	menuTexture = loadImage("resources/Menu.png");
	playTexture = loadImage("resources/Play.png");
	playSelectedTexture = loadImage("resources/Play selected.png");
	modeTexture = loadImage("resources/Mode.png");
	modeSelectedTexture = loadImage("resources/Mode selected.png");
	optionTexture = loadImage("resources/Option.png");
	optionSelectedTexture = loadImage("resources/Option selected.png");
}

void drawMenu(bool keyboardState[256]) {
	glBindTexture(GL_TEXTURE_2D, menuTexture);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(0, 0);
	glTexCoord2i(0, 1); glVertex2i(0, 1);
	glTexCoord2i(1, 1); glVertex2i(1, 1);
	glTexCoord2i(1, 0); glVertex2i(1, 0);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
}