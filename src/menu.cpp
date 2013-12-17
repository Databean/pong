#include "menu.h"

#include "pong.h"
#include "image.h"

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

int selected = 0;

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
	
	difficultyText = {{0.5, 0.5}, {550/1280., 150/720.}, {0, 0}, {1, 1, 1}, difficultyTexture};
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