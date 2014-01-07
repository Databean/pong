#include "Powerup.h"

#include <random>

#include "PaddleBehavior.h"

std::mt19937 rd;
std::mt19937 randomgen(rd());
std::uniform_real_distribution<float> positionDistr(0, 1);
std::uniform_int_distribution<int> powerupTypeDistr(0, 0);

std::unique_ptr<Powerup> randomPowerup() {
	return std::unique_ptr<Powerup>(new PowerupStopVertical(positionDistr(randomgen), positionDistr(randomgen)));
}

Powerup::Powerup(const object& obj) : obj(obj) {
	
}

void Powerup::draw() {
	displayObject(obj);
}

object& Powerup::getObject() {
	return obj;
}

bool Powerup::isColliding(object& ball) const {
	return ball.intersects(obj);
}

PowerupStopVertical::PowerupStopVertical(float x, float y) : Powerup({{x, y}, {0.1, 0.16}, {0, 0}, {1.0, 0.0, 0.0}, 0}) {
	
}

void PowerupStopVertical::trigger(PaddleBehavior& left, PaddleBehavior& right, object& ball) {
	ball.vel.y = 0;
}



