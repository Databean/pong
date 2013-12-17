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
object playButton = {{175/1280., 0.6}, {350./1280.,150./720.}, {0, 0}, {1, 1, 1}, playTexture};
object modeButton = {{175/1280., 0.4}, {350./1280.,150./720.}, {0, 0}, {1, 1, 1}, modeTexture};
object optionButton = {{175/1280., 0.2}, {350./1280.,150./720.}, {0, 0}, {1, 1, 1}, optionTexture};

int selected = 0;

void initMenu() {
	menuTexture = loadImage("resources/Menu.png");
	playTexture = loadImage("resources/Play.png");
	playSelectedTexture = loadImage("resources/Play selected.png");
	modeTexture = loadImage("resources/Mode.png");
	modeSelectedTexture = loadImage("resources/Mode selected.png");
	optionTexture = loadImage("resources/Option.png");
	optionSelectedTexture = loadImage("resources/Option selected.png");
	
	bgObject.texture = menuTexture;
	playButton.texture = playTexture;
	modeButton.texture = modeTexture;
	optionButton.texture = optionTexture;
}

void drawMenu(bool keyboardState[256]) {
	
	static bool previousUp, previousDown;
	
	if(keyboardState['w'] && !previousUp) {
		selected = (selected + 3 - 1) % 3;
	}
	if(keyboardState['s'] && ! previousDown) {
		selected = (selected + 1) % 3;
	}
	
	previousUp = keyboardState['w'];
	previousDown = keyboardState['s'];
	
	playButton.texture = selected == 0 ? playSelectedTexture : playTexture;
	modeButton.texture = selected == 1 ? modeSelectedTexture : modeTexture;
	optionButton.texture = selected == 2 ? optionSelectedTexture : optionTexture;
	
	displayObject(bgObject);
	displayObject(playButton);
	displayObject(modeButton);
	displayObject(optionButton);
	
	if(selected == 0 && keyboardState[' ']) {
		showGame();
	}
}