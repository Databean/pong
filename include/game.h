#ifndef PONG_GAME_H
#define PONG_GAME_H

void initGame();

float limit(float min, float max, float x);

void newGame();

void movementLogic(bool keyboardState[256]);

void drawGame();

#endif