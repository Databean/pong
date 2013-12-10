#include "menu.h"

#include "pong.h"
#include "image.h"

GLuint menuTexture;
GLuint playTexture;
GLuint playSelectedTexture;
GLuint modeTexture;
GLuint modeSelectedTexture;
GLuint optionTexture;
GLuint optionSelectedTexture;

object bgObject = {{0.5, 0.5}, {1, 1}, {0, 0}, {1, 1, 1}, menuTexture};

void initMenu() {
	menuTexture = loadImage("resources/Menu.png");
	playTexture = loadImage("resources/Play.png");
	playSelectedTexture = loadImage("resources/Play selected.png");
	modeTexture = loadImage("resources/Mode.png");
	modeSelectedTexture = loadImage("resources/Mode selected.png");
	optionTexture = loadImage("resources/Option.png");
	optionSelectedTexture = loadImage("resources/Option selected.png");
	
	bgObject.texture = menuTexture;
}

void drawMenu(bool keyboardState[256]) {
	
	displayObject(bgObject);
}