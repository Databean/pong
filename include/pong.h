#ifndef PONG_H
#define PONG_H

#include <GL/gl.h>

#include <cmath>

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
	GLuint texture;
	
	bool contains(float x, float y) const;
	
	// doesn't work with complex intersections, but don't need those here.
	bool intersects(const object& o) const;
	
	bool inBounds() const;
};

void displayObject(const object&);

void showGame();
void showMenu();

#endif