#ifndef PADDLE_BEHAVIOR_H
#define PADDLE_BEHAVIOR_H

#include "pong.h"

class PaddleBehavior {
protected:
	object& paddle;
public:
	PaddleBehavior(object& paddle);
	PaddleBehavior(const PaddleBehavior&) = delete;
	virtual ~PaddleBehavior();
	PaddleBehavior& operator=(const PaddleBehavior&) = delete;
	
	object& getPaddle();
	virtual void doMovement(bool keyboardState[256]) = 0;
	virtual void doAI(const object& ball) = 0;
};

class StandardPaddle : public PaddleBehavior {
private:
	float speed;
public:
	StandardPaddle(object& paddle, float speed);
	StandardPaddle(const StandardPaddle&) = delete;
	virtual ~StandardPaddle();
	StandardPaddle& operator=(const StandardPaddle&) = delete;
	
	virtual void doMovement(bool keyboardState[256]);
	virtual void doAI(const object& ball);
};

class JumpPaddle : public PaddleBehavior {
private:
	float speed;
	float dashSpeed;
	
	float dash;
public:
	JumpPaddle(object& paddle, float speed, float dashSpeed);
	JumpPaddle(const JumpPaddle&) = delete;
	virtual ~JumpPaddle();
	JumpPaddle& operator=(const JumpPaddle&) = delete;
	
	virtual void doMovement(bool keyboardState[256]);
	virtual void doAI(const object& ball);
};



#endif