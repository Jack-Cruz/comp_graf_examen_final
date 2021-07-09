#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#include <typedef.h>
#include <tuple>

#include "resource.h"
#include "sprite.h"
#include "game_level.h"
#include "ball_object.h"


// Represents the current state of the game
enum GameState { 
	GAME_ACTIVE, 
	GAME_MENU, 
	GAME_WIN 
};

enum class Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT,
	FRONT,
	BACK
};

// Colission ocurred, direction it ocurred and the difference vector R
typedef std::tuple<bool, Direction, glm::vec3> Collision;

// initial size of the player paddle
const glm::vec3 PLAYER_SIZE(150.0f, 60.0f, 150.0f);
const float PLAYER_VELOCITY(500.0f);

const glm::vec3 INITIAL_BALL_VELOCITY(0.0f, 30.0f, 0.0f);
const float BALL_RADIUS = 5.0f;

// Game-related state data
SpriteRenderer* Renderer;
GameObject* Player;

// Holds game-related state and functionallity.
// combines game-related data into a class
// components and manageability.
class Game {
public:
	// game state
	GameState    State;
	bool         Keys[1024];
	unsigned int Width, Height, Depth;
	std::vector<GameLevel> Levels;
	ui32 Level;
	vector<BallObject*> balas_jugador;


	// constructor / destructur
	Game(unsigned int width, unsigned int height, unsigned int depth)
		: State(GAME_ACTIVE), Keys(), Width(width), Height(height), Depth(depth) {}
	~Game() {
		delete Renderer;
		delete Player;
		//ResourceManager::Clear();
	}
	
	// initialize game state (load all shaders/textures/level)
	void Init() {
		// load shader
		ResourceManager::LoadShader("sprite", "shader.vert", "shader.frag", 
			"D:/JC/UPC 2021-01/Comp. Graf/JackCruz/jack/jack");
		
		// configure shader
		ResourceManager::GetShader("sprite")->use()->setI32("image", 0);

		// set render-specific controls
		Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

		// load textures
		ResourceManager::LoadTexture("background.jpg", "D:/JC/UPC 2021-01/Comp. Graf/JackCruz/Git OpenGL/resources/textures", false, "background");
		ResourceManager::LoadTexture("mafalda.jpg", "D:/JC/UPC 2021-01/Comp. Graf/JackCruz/Git OpenGL/resources/textures", false, "mafalda");
		ResourceManager::LoadTexture("awesome_face.png", "D:/JC/UPC 2021-01/Comp. Graf/JackCruz/Git OpenGL/resources/textures", true, "face");
		ResourceManager::LoadTexture("block.png", "D:/JC/UPC 2021-01/Comp. Graf/JackCruz/Git OpenGL/resources/textures", false, "block");
		ResourceManager::LoadTexture("block_solid.png", "D:/JC/UPC 2021-01/Comp. Graf/JackCruz/Git OpenGL/resources/textures", false, "block_solid");
		ResourceManager::LoadTexture("paddle.png", "D:/JC/UPC 2021-01/Comp. Graf/JackCruz/Git OpenGL/resources/textures", true, "paddle");

		// load levels
		GameLevel one; one.Load("D:/JC/UPC 2021-01/Comp. Graf/JackCruz/jack/jack/level/one.lvl", Width, Height, Depth, 1);
		GameLevel two; two.Load("D:/JC/UPC 2021-01/Comp. Graf/JackCruz/jack/jack/level/two.lvl", Width, Height, Depth, 2);
		GameLevel three; three.Load("D:/JC/UPC 2021-01/Comp. Graf/JackCruz/jack/jack/level/three.lvl", Width, Height, Depth, 3);
		GameLevel four; four.Load("D:/JC/UPC 2021-01/Comp. Graf/JackCruz/jack/jack/level/four.lvl", Width, Height, Depth, 4);

		Levels.push_back(one);
		Levels.push_back(two);
		Levels.push_back(three);
		Levels.push_back(four);
		Level = 0;

		glm::vec3 playerPos = glm::vec3(1.0f);
		playerPos.x = Width / 2.0f;
		playerPos.y = PLAYER_SIZE.y / 2.0f;
		playerPos.z = Depth / 2.0f;

		Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	}
	
	// game loop (deltatime)
	void ProcessInput(f32 dt) {
		if (State == GAME_ACTIVE) {
			float velocity = PLAYER_VELOCITY * dt;

			// move playerboard
			if (Keys[GLFW_KEY_LEFT]) {
				if (Player->Position.x >= Player->Size.x / 2.0f)
					Player->Position.x -= velocity;
			}
			if (Keys[GLFW_KEY_RIGHT]) {
				if (Player->Position.x <= Width - Player->Size.x /2.0f)
					Player->Position.x += velocity;
			}
			if (Keys[GLFW_KEY_DOWN]) {
				if (Player->Position.z >= Player->Size.z / 2.0f)
					Player->Position.z -= velocity;
			}
			if (Keys[GLFW_KEY_UP]) {
				if (Player->Position.z <= Depth - Player->Size.z / 2.0f)
					Player->Position.z += velocity;
			}
			if (Keys[GLFW_KEY_SPACE]) {

				glm::vec3 ballPos = glm::vec3(1.0f);
				ballPos.x = 0.0f;
				ballPos.y = PLAYER_SIZE.y / 2.0f + BALL_RADIUS;
				ballPos.z = 0.0f;

				ballPos = Player->Position + ballPos;


				BallObject* Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
					ResourceManager::GetTexture("face"));

				balas_jugador.push_back(Ball);

				// Ball->Stuck = false;
			}
		}
	}
	void Update(f32 dt) {

		// Update objects
		for (auto* ball : balas_jugador) {
			if (!ball->Stuck)
			{
				if (ball->Position.y >= Height) {
					ball->Stuck = true;
				}

				else {
					ball->Move(dt, Width, Height, Depth, Player->Position.x, Player->Position.z);
				}
			}
		}


		//check for collisions
		for (auto* ball : balas_jugador) {
			if (!ball->Stuck) {
				DoCollisions(ball);
			}
		}
		// Check bottom edge
		/* if (Ball->Position.y <= 0.0f) {
			ResetLevel();
			ResetPlayer();
		}
		*/
	}

	void ResetLevel() {
		if (this->Level == 0) {
			this->Levels[0].Load("D:/JC/UPC 2021-01/Comp. Graf/JackCruz/jack/jack/level/one.lvl", Width, Height, Depth, 1);
		}
		else if (this->Level == 1) {
			this->Levels[1].Load("D:/JC/UPC 2021-01/Comp. Graf/JackCruz/jack/jack/level/two.lvl", Width, Height, Depth, 2);
		}
		else if (this->Level == 2) {
			this->Levels[2].Load("D:/JC/UPC 2021-01/Comp. Graf/JackCruz/jack/jack/level/three.lvl", Width, Height, Depth, 3);
		}
		else if (this->Level == 3) {
			this->Levels[3].Load("D:/JC/UPC 2021-01/Comp. Graf/JackCruz/jack/jack/level/four.lvl", Width, Height, Depth, 4);
		}
	}
	void ResetPlayer(){
		// reset player/ball stats
		Player->Size = PLAYER_SIZE;
		
		Player->Position.x = Width / 2.0f;
		Player->Position.y = PLAYER_SIZE.y / 2.0f;
		Player->Position.z = Depth / 2.0f;

		vec3 ballPos = vec3(1.0f);

		ballPos.x = 0.0f;
		ballPos.y = PLAYER_SIZE.y / 2.0f + BALL_RADIUS;
		ballPos.z = 0.0f;

		ballPos = Player->Position + ballPos;

		//Ball->Reset(ballPos, INITIAL_BALL_VELOCITY);
	}

	void Render() {
		if (State == GAME_ACTIVE) {
			// draw background
			// Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			//	glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(Width, Height, 0.0f), 0.0f);

			// draw the level
			Levels[0].Draw(Renderer);
			Levels[1].Draw(Renderer);
			Levels[2].Draw(Renderer);
			Levels[3].Draw(Renderer);

			// draw the player
			Player->Draw(Renderer);

			// draw the ball
			for (auto* ball : balas_jugador) {
				if (!ball->Stuck) {
					ball->Draw(Renderer);
				}
			}
		}
	}

	// check between ball and each brick of the level 
	/*bool CheckCollision(GameObject* one, GameObject* two) // AABB - AABB
	{
		// collision x-axis ?
		bool collisionX = one->Position.x + one->Size.x >= two->Position.x &&
			two->Position.x + two->Size.x >= one->Position.x;
		
		// collision y-axis ?
		bool collisionY = one->Position.y + one->Size.y >= two->Position.y &&
			two->Position.y + two->Size.y >= one->Position.y;

		// collision only if on both axes
		return collisionX && collisionY;
	}*/

	Direction VectorDirection(glm::vec3 target) {
		glm::vec3 compass[] = {
			glm::vec3(0.0f,  1.0f, 0.0f), // up
			glm::vec3(1.0f,  0.0f, 0.0f), // right
			glm::vec3(0.0f, -1.0f, 0.0f), // down
			glm::vec3(-1.0f,  0.0f, 0.0f), // left
			glm::vec3(0.0,  0.0f, 1.0f), // front
			glm::vec3(0.0,  0.0f, -1.0f) // front
		};

		float max = 0.0f;

		unsigned int best_match = -1;
		for (ui32 i = 0; i < 6; i++) {
			float dot_product = glm::dot(glm::normalize(target), compass[i]);
			if (dot_product > max) {
				max = dot_product;
				best_match = i;
			}
		}
		return (Direction)best_match;
	}

	bool heckCollision(GameObject* one, GameObject* two) // AABB - AABB collision
	{
		// collision x-axis?
		bool collisionX = one->Position.x + one->Size.x >= two->Position.x &&
			two->Position.x + two->Size.x >= one->Position.x;

		// collision y-axis?
		bool collisionY = one->Position.y + one->Size.y >= two->Position.y &&
			two->Position.y + two->Size.y >= one->Position.y;

		// collision only if on both axes
		return collisionX && collisionY;
	}
	
	Collision CheckCollision(BallObject* one, GameObject* two) // AABB - Circle
	{
		// get center point circle first
		glm::vec3 center(one->Position);

		// calculate AABB info (center, half-extends)
		glm::vec3 aabb_half_extends(0.5f * two->Size);
		glm::vec3 aabb_center(two->Position);

		// get the diference vector between both centers
		glm::vec3 difference = center - aabb_center;
		glm::vec3 clamped = glm::clamp(difference, -aabb_half_extends, aabb_half_extends);

		// add clamped value to AABB_center and get the value closest to circle
		glm::vec3 closest = aabb_center + clamped;

		// vector between center circle and closest point AABB
		difference = closest - center;

		if (glm::length(difference) < one->Radius) {
			return std::make_tuple(true, VectorDirection(difference), difference);
		}
		else{ 
			return std::make_tuple(false, Direction::UP, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
	
	void DoCollisions(BallObject* Ball) {
		// Collision de ladrillos
		for (ui32 i = 0; i < 4; i++) { // levels
			for (GameObject* box : Levels[i].Bricks) {

				if (!box->Destroyed) {
					Collision  collision = CheckCollision(Ball, box);

					if (std::get<0>(collision)) { // if collision is true
						// destroy the block if not solid
						box->Destroyed = true;
						Ball->Stuck = true;
					}
				}
			}
		}
	}
		// collision de jugador
		/*
		Collision result = CheckCollision(Ball, Player);
		if (!Ball->Stuck && std::get<0>(result)) {
			
			// check where it hit the board, and change velocity
			vec3 centerBoard = Player->Position;
			float strength = 2.0f;
			
			// colision x
			float distancex = Ball->Position.x - centerBoard.x;
			float percentagex = distancex / (Player->Size.x / 2.0f);

			// colision z
			float distancez = Ball->Position.z - centerBoard.z;
			float percentagez = distancez / (Player->Size.z / 2.0f);

			// then move accordingly
			glm::vec2 oldVelocity = Ball->Velocity;
			Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentagex * strength;
			Ball->Velocity.z = INITIAL_BALL_VELOCITY.z * percentagez * strength;

			Ball->Velocity.y = 1.0f * abs(Ball->Velocity.y);
 			Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
		
		}
		*/
	
	
	
};
#endif
