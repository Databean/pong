#ifndef PONG_MENU_H
#define PONG_MENU_H

#include "GL/gl.h"

#include <vector>

#include "pong.h"

void initMenu();

void drawMenu(bool keyboardState[256]);

class MenuItem {
private:
	object obj;
	GLuint unselectedTexture;
	GLuint selectedTexture;
public:
	MenuItem(const object& obj, GLuint unselectedTexture, GLuint selectedTexture);
	MenuItem(const MenuItem&) = default;
	~MenuItem() = default;
	MenuItem& operator=(const MenuItem&) = default;
	
	void select();
	void unselect();
	object& getObject();
};

class Menu {
private:
	int selectedRow;
	int selectedColumn;
	std::vector<std::vector<MenuItem>> menuItems;
	object background;
public:
	Menu(const object& background);
	Menu(const Menu&) = default;
	~Menu() = default;
	Menu& operator=(const Menu&) = default;
	
	void setRows(int rows);
	void setColumns(int row, int column);
	MenuItem& operator()(int row, int column);
	void select(int row, int column);
	void draw();
};

#endif