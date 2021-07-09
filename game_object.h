#pragma once

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <typedef.h>
#include <texture.h>
#include "sprite.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.

class GameObject {
public:
	// Object state
	glm::vec3 Position, Size, Velocity;
	glm::vec3 Color;
	f32 Rotation;
	bool IsSolid;
	bool Destroyed;

	// render state
	Texture* Sprite;

	// constructor
	GameObject()
		:Position({ 0.0f, 0.0f, 0.0 }), Size({ 1.0f, 1.0f, 1.0f }), Color(vec3(1.0f)), Rotation(0.0f), Sprite(), IsSolid(false),
		Destroyed(false){}

	GameObject(glm::vec3 pos, glm::vec3 size, Texture* sprite, glm::vec3 color = glm::vec3(1.0f),
		glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f))
	: Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false),
	Destroyed(false){}
	
	// draw sprite
	void Draw(SpriteRenderer* renderer) {
		renderer->DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
	}

};

#endif