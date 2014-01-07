#ifndef POWERUP_H
#define POWERUP_H

#include <memory>

#include "pong.h"

class PaddleBehavior;
class Powerup;

std::unique_ptr<Powerup> randomPowerup();

class Powerup {
private:
	object obj;
public:
	Powerup(const object& obj);
	Powerup(const Powerup&) = delete;
	virtual ~Powerup() = default;
	virtual Powerup& operator=(const Powerup&) = delete;
	
	virtual void draw();
	virtual void trigger(PaddleBehavior& leftPaddle, PaddleBehavior& rightPaddle, object& ball) = 0;
	virtual object& getObject();
	virtual bool isColliding(object& ball) const;
};

class PowerupStopVertical : public Powerup {
public:
	PowerupStopVertical(float x, float y);
	PowerupStopVertical(const PowerupStopVertical&) = delete;
	virtual ~PowerupStopVertical() = default;
	virtual PowerupStopVertical& operator=(const PowerupStopVertical&) = delete;
	
	virtual void trigger(PaddleBehavior& leftPaddle, PaddleBehavior& rightPaddle, object& ball);
};

#endif