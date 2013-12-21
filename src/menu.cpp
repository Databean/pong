#include "menu.h"

#include <GL/freeglut.h>

#include "pong.h"
#include "image.h"

enum { MAIN_MENU, MODE_MENU } menuScreen;

GLuint menuBackground;
GLuint playTexture;
GLuint playSelectedTexture;
GLuint modeTexture;
GLuint modeSelectedTexture;
GLuint optionTexture;
GLuint optionSelectedTexture;

GLuint modeBackground;
GLuint difficultyTexture;
GLuint easyTexture;
GLuint easySelectedTexture;
GLuint mediumTexture;
GLuint mediumSelectedTexture;
GLuint hardTexture;
GLuint hardSelectedTexture;
GLuint extremeTexture;
GLuint extremeSelectedTexture;

object bgObject;
object playButton;
object modeButton;
object optionButton;

object difficultyText;
object easyButton;
object mediumButton;
object hardButton;
object extremeButton;

bool previousEscape = false;

void initMenu() {
	menuBackground = loadImage("resources/Menu.png");
	playTexture = loadImage("resources/Play.png");
	playSelectedTexture = loadImage("resources/Play selected.png");
	modeTexture = loadImage("resources/Mode.png");
	modeSelectedTexture = loadImage("resources/Mode selected.png");
	optionTexture = loadImage("resources/Option.png");
	optionSelectedTexture = loadImage("resources/Option selected.png");
	
	modeBackground = loadImage("resources/Mode Menu.png");
	difficultyTexture = loadImage("resources/Difficulty.png");
	easyTexture = loadImage("resources/Easy.png");
	easySelectedTexture = loadImage("resources/Easy Selected.png");
	mediumTexture = loadImage("resources/Medium.png");
	mediumSelectedTexture = loadImage("resources/Medium Selected.png");
	hardTexture = loadImage("resources/Hard.png");
	hardSelectedTexture = loadImage("resources/Hard Selected.png");
	extremeTexture = loadImage("resources/Extreme.png");
	extremeSelectedTexture = loadImage("resources/Extreme Selected.png");
	
	bgObject = {{0.5, 0.5}, {1, 1}, {0, 0}, {1, 1, 1}, menuBackground};
	playButton = {{175/1280., 0.6}, {350./1280.,150./720.}, {0, 0}, {1, 1, 1}, playTexture};
	modeButton = {{175/1280., 0.4}, {350./1280.,150./720.}, {0, 0}, {1, 1, 1}, modeTexture};
	optionButton= {{175/1280., 0.2}, {350./1280.,150./720.}, {0, 0}, {1, 1, 1}, optionTexture};
	
	difficultyText = {{275/1280., 0.75}, {550/1280., 150/720.}, {0, 0}, {1, 1, 1}, difficultyTexture};
	easyButton = {{175/1280., 0.6}, {350/1280., 150/720.}, {0, 0}, {1, 1, 1}, easyTexture};
	mediumButton = {{425/1280., 0.6}, {350/1280., 150/720.}, {0, 0}, {1, 1, 1}, mediumTexture};
	hardButton = {{750/1280., 0.6}, {350/1280., 150/720.}, {0, 0}, {1, 1, 1}, hardTexture};
	extremeButton = {{1000/1280., 0.6}, {350/1280., 150/720.}, {0, 0}, {1, 1, 1}, extremeTexture};
}

void drawMainMenu(bool keyboardState[256]) {
	static bool previousUp, previousDown;
	
	static int selected;
	
	bgObject.texture = menuBackground;
	
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
	
	if(keyboardState[' ']) {
		if(selected == 0) {
			showGame();
		} else if(selected == 1) {
			menuScreen = MODE_MENU;
		}
	} else if(keyboardState[27] && !previousEscape) { //Escape
		glutLeaveMainLoop();
	}
}

void drawModeMenu(bool keyboardState[256]) {
	static bool previousLeft, previousRight;
	
	bgObject.texture = modeBackground;
	
	static int column = 0;
	
	if(keyboardState['a'] && !previousLeft) {
		column = (column + 4 - 1) % 4;
	}
	if(keyboardState['d'] && !previousRight) {
		column = (column + 1) % 4;
	}
	
	previousLeft = keyboardState['a'];
	previousRight = keyboardState['d'];
	
	easyButton.texture = column == 0 ? easySelectedTexture : easyTexture;
	mediumButton.texture = column == 1 ? mediumSelectedTexture : mediumTexture;
	hardButton.texture = column == 2 ? hardSelectedTexture : hardTexture;
	extremeButton.texture = column == 3 ? extremeSelectedTexture : extremeTexture;
	
	displayObject(bgObject);
	displayObject(difficultyText);
	displayObject(easyButton);
	displayObject(mediumButton);
	displayObject(hardButton);
	displayObject(extremeButton);
	
	if(keyboardState[27] && !previousEscape) { //Escape
		menuScreen = MAIN_MENU;
	}
}

void drawMenu(bool keyboardState[256]) {
	switch(menuScreen) {
		case MAIN_MENU:
			drawMainMenu(keyboardState);
			break;
		case MODE_MENU:
			drawModeMenu(keyboardState);
			break;
	}
	
	previousEscape = keyboardState[27];
}
