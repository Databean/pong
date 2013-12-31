#include "menu.h"

#include <GL/freeglut.h>

#include "pong.h"
#include "image.h"
#include "Settings.h"

using std::string;

enum { MAIN_MENU, MODE_MENU } menuScreen;

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

MenuItem::MenuItem(const object& obj, GLuint unselectedTexture, GLuint selectedTexture) : obj(obj), unselectedTexture(unselectedTexture), selectedTexture(selectedTexture) {
	this->obj.texture = unselectedTexture;
}

object& MenuItem::getObject() {
	return obj;
}

void MenuItem::select() {
	obj.texture = selectedTexture;
}

void MenuItem::unselect() {
	obj.texture = unselectedTexture;
}

Menu::Menu(const object& background) : background(background) {
	
}

void Menu::setRows(int rows) {
	menuItems.resize(rows);
}

void Menu::setColumns(int row, int columns) {
	static object dummyObject = {{0, 0}, {0, 0}, {0, 0}, {0, 0, 0}};
	static MenuItem dummyMenu(dummyObject, 0, 0);
	menuItems[row].resize(columns, dummyMenu);
}

MenuItem& Menu::operator()(int row, int column) {
	return menuItems[row][column];
}

void Menu::select(int row, int column) {
	for(std::vector<MenuItem>& menuRow : menuItems) {
		for(MenuItem& menuItem : menuRow) {
			menuItem.unselect();
		}
	}
	selectedRow = row;
	selectedColumn = column;
	menuItems[row][column].select();
}

void Menu::draw() {
	displayObject(background);
	for(std::vector<MenuItem>& menuRow : menuItems) {
		for(MenuItem& menuItem : menuRow) {
			displayObject(menuItem.getObject());
		}
	}
}

Menu* mainMenu;
Menu* modeMenu;

bool previousEscape = false;

void initMenu() {
	mainMenu = new Menu({{0.5, 0.5}, {1, 1}, {0, 0}, {1, 1, 1}, loadImage("resources/Menu.png")});
	mainMenu->setRows(3);
	for(int i = 0; i < 3; i++) { mainMenu->setColumns(i, 1); }
	(*mainMenu)(0, 0) = MenuItem({{175/1280., 0.6}, {350./1280.,150./720.}, {0, 0}, {1, 1, 1}, 0}, loadImage("resources/Play.png"), loadImage("resources/Play selected.png"));
	(*mainMenu)(1, 0) = MenuItem({{175/1280., 0.4}, {350./1280.,150./720.}, {0, 0}, {1, 1, 1}, 0}, loadImage("resources/Mode.png"), loadImage("resources/Mode selected.png"));
	(*mainMenu)(2, 0) = MenuItem({{175/1280., 0.2}, {350./1280.,150./720.}, {0, 0}, {1, 1, 1}, 0}, loadImage("resources/Option.png"), loadImage("resources/Option selected.png"));
	
	modeMenu = new Menu({{0.5, 0.5}, {1, 1}, {0, 0}, {1, 1, 1}, loadImage("resources/Mode Menu.png")});
	modeMenu->setRows(1);
	modeMenu->setColumns(0, 4);
	(*modeMenu)(0, 0) = MenuItem({{175/1280., 0.6}, {350/1280., 150/720.}, {0, 0}, {1, 1, 1}, 0}, loadImage("resources/Easy.png"), loadImage("resources/Easy Selected.png"));
	(*modeMenu)(0, 1) = MenuItem({{425/1280., 0.6}, {350/1280., 150/720.}, {0, 0}, {1, 1, 1}, 0}, loadImage("resources/Medium.png"), loadImage("resources/Medium Selected.png"));
	(*modeMenu)(0, 2) = MenuItem({{750/1280., 0.6}, {350/1280., 150/720.}, {0, 0}, {1, 1, 1}, 0}, loadImage("resources/Hard.png"), loadImage("resources/Hard Selected.png"));
	(*modeMenu)(0, 3) = MenuItem({{1000/1280., 0.6}, {350/1280., 150/720.}, {0, 0}, {1, 1, 1}, 0}, loadImage("resources/Extreme.png"), loadImage("resources/Extreme Selected.png"));
	
	difficultyTexture = loadImage("resources/Difficulty.png");
	
	difficultyText = {{275/1280., 0.75}, {550/1280., 150/720.}, {0, 0}, {1, 1, 1}, difficultyTexture};
}

void drawMainMenu(bool keyboardState[256]) {
	static bool previousUp, previousDown;
	
	static int selected;
	
	if(keyboardState['w'] && !previousUp) {
		selected = (selected + 3 - 1) % 3;
	}
	if(keyboardState['s'] && ! previousDown) {
		selected = (selected + 1) % 3;
	}
	
	previousUp = keyboardState['w'];
	previousDown = keyboardState['s'];
	
	mainMenu->select(selected, 0);
	mainMenu->draw();
	
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
	
	static int column = 0;
	
	const string& difficulty = settings.get("difficulty","medium");
	if(difficulty == "easy") {
		column = 0;
	} else if(difficulty == "medium") {
		column = 1;
	} else if(difficulty == "hard") {
		column = 2;
	} else if(difficulty == "extreme") {
		column = 3;
	} else {
		column = 1;
	}
	
	if(keyboardState['a'] && !previousLeft) {
		column = (column + 4 - 1) % 4;
	}
	if(keyboardState['d'] && !previousRight) {
		column = (column + 1) % 4;
	}
	
	if(column == 0) {
		settings.set("difficulty", "easy");
	} else if(column == 1) {
		settings.set("difficulty", "medium");
	} else if(column == 2) {
		settings.set("difficulty", "hard");
	} else if(column == 3) {
		settings.set("difficulty", "extreme");
	}
	
	previousLeft = keyboardState['a'];
	previousRight = keyboardState['d'];
	
	modeMenu->select(0, column);
	modeMenu->draw();
	
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
