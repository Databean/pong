#ifndef PONG_GAME_H
#define PONG_GAME_H

float limit(float min, float max, float x);

void movementLogic(bool keyboardState[256]);

void drawGame();

#endif