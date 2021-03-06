#include "PaddleBehavior.h"

#include <cmath>

#include <iostream>

#include "util.h"

using std::abs;

PaddleBehavior::PaddleBehavior(object& paddle) : paddle(paddle) {
	
}

PaddleBehavior::~PaddleBehavior() {
	
}

object& PaddleBehavior::getPaddle() {
	return paddle;
}

StandardPaddle::StandardPaddle(object& paddle, float speed) : PaddleBehavior(paddle), speed(speed) {
	
}

StandardPaddle::~StandardPaddle() {
	
}

void StandardPaddle::doMovement(bool keyboardState[256]) {
	float speed = 0.01;
	if(keyboardState['s'] && paddle.pos.y - paddle.dim.y/2.f > 0.f) {
		paddle.vel.y = -speed;
	}
	else if(keyboardState['w'] && paddle.pos.y + paddle.dim.y/2.f < 1.f) {
		paddle.vel.y = speed;
	} else {
		paddle.vel.y = 0;
	}
}

void StandardPaddle::doAI(const object& ball) {
	if(abs(paddle.pos.y - ball.pos.y) < speed) {
		paddle.vel.y = ball.pos.y - paddle.pos.y;
	} else if(paddle.pos.y > ball.pos.y) {
		paddle.vel.y = -speed;
	} else if(paddle.pos.y < ball.pos.y) {
		paddle.vel.y = speed;
	} else {
		paddle.vel.y = 0;
	}
}

JumpPaddle::JumpPaddle(object& paddle, float speed, float dashSpeed) : PaddleBehavior(paddle), speed(speed), dashSpeed(dashSpeed) {
	dash = 0.0;
}

JumpPaddle::~JumpPaddle() {
	
}

void JumpPaddle::doMovement(bool keyboardState[256]) {
	if(dash > .2) {
		if(paddle.pos.y - paddle.dim.y/2.f > 0.f) {}
		else if(paddle.pos.y + paddle.dim.y/2.f < 1.f) {}
		else {
			paddle.vel.y = 0;
		}
	} else if(dash > 0) {
		paddle.vel.y = 0;
	} else {
		paddle.vel.y = 0;
		if(keyboardState['s']) {
			paddle.vel.y = -speed;
		}
		else if(keyboardState['w']) {
			paddle.vel.y = speed;
		} else {
			paddle.vel.y = 0;
		}
		
		if(keyboardState[' '] && signum(paddle.vel.y) != 0) {
			int dir = signum(paddle.vel.y);
			dash = 0.26;
			paddle.vel.y = dashSpeed * dir;
		}
	}
	
	dash -= 0.01;
}

void JumpPaddle::doAI(const object& ball) {
	if(dash > .2) {
		
	} else if(dash > 0) {
		paddle.vel.y = 0;
	} else {
		bool shouldDash = false;
		if(signum(paddle.pos.x - ball.pos.x) == signum(ball.vel.x)) {
			float ballY = ball.pos.y;
			float ballVel = ball.vel.y;
			float paddleY = paddle.pos.y;
			for(float x = ball.pos.x; x < paddle.pos.x; x += ball.vel.x) {
				if(ballY >= 1 || ballY <= 0) {
					ballVel *= -1.f;
				}
				ballY += ballVel;
				paddleY += (ballY > paddleY ? speed : -speed);
			}
			float time = (paddle.pos.x - ball.pos.x) / ball.vel.x;
			float dashNowY = paddleY + (ballY > paddleY ? dashSpeed : -dashSpeed) * time;
			
			if(abs(paddleY - ballY) > paddle.dim.y / 2.f && abs(dashNowY - ballY) < paddle.dim.y / 2.f) {
				shouldDash = true;
			}
		}
		if(shouldDash && abs(paddle.pos.x - ball.pos.x) < 0.06) {
			int dir = -signum(paddle.pos.y - ball.pos.y);
			dash = 0.26;
			paddle.vel.y = dashSpeed * dir;
		} else if(abs(paddle.pos.y - ball.pos.y) < speed) {
			paddle.vel.y = ball.pos.y - paddle.pos.y;
		} else if(paddle.pos.y > ball.pos.y) {
			paddle.vel.y = -speed;
		} else if(paddle.pos.y < ball.pos.y) {
			paddle.vel.y = speed;
		} else {
			paddle.vel.y = 0;
		}
	}
	
	dash -= 0.01;
}

