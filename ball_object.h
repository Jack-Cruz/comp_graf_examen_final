#pragma once

#ifndef __BALL_OBJECT__
#define __BALL_OBJECT__

#include "game_object.h"

class BallObject : public GameObject {
public:
	// ball state
	float Radius;
	bool Stuck;

	// constructor
	BallObject()
	:GameObject(), Radius(12.5f), Stuck(true){}

	BallObject(glm::vec3 pos, float radius, glm::vec3 velocity, Texture* sprite)
	:GameObject(pos, glm::vec3(radius *2.0f, radius*2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity),
		Radius(radius), Stuck(false){}

	// move the ball, returns new position
	void Move(float dt, unsigned int window_width, unsigned int window_height, unsigned int window_depth, ui32 player_x, ui32 player_z) {
		if (!Stuck) {
			// move the ball
			Position += Velocity * dt;

			// check if outside window bounds; if so, reverse velocity
			// and restore at correct position

			if (Position.x <= Radius) {
				Velocity.x = -Velocity.x;
				Position.x = Radius;
			}
			else if (Position.x >= window_width - Radius) {
				Velocity.x = -Velocity.x;
				Position.x = window_width - Radius;
			}

			if (Position.z <= Radius) {
				Velocity.z = -Velocity.z;
				Position.z = Radius;
			}
			else if (Position.z >= window_depth - Radius) {
				Velocity.z = -Velocity.z;
				Position.z = window_depth - Radius;
			}

			if (Position.y >= window_height - Radius) {
				Velocity.y = -Velocity.y;
				Position.y = window_height - Radius;
			}
		}
	}
	
	// resets the ball to original stuck position
	void Reset(glm::vec3 position, glm::vec3 velocity) {
		this->Position = position;
		this->Velocity = velocity;
		this->Stuck = true;
	}
};
 
#endif