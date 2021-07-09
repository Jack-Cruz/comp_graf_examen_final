#pragma once
#ifndef __GAMELEVEL_H__
#define __GAMELEVEL_H__

#include "game_object.h"
#include "ball_object.h"
#include "resource.h"

#include <typedef.h>
#include <texture.h>

#include <fstream>
#include <sstream>

class GameLevel {
	
public:
	// level state
	std::vector<GameObject*> Bricks;

	// constructor
	GameLevel() {}
	
	// loads level from file
	void Load(const char* file, ui32 levelWidth, ui32 levelHeight, ui32 levelDepth, ui32 nivel) {
		// clear old data
		// Bricks.clear();

		// load from file
		ui32 tileCode;
		GameLevel level;
		std::string line;
		std::ifstream fstream(file);
		std::vector<std::vector<ui32>> tileData;

		if (fstream) {
			while (std::getline(fstream, line)) // read each line from file
			{
				 // TODO
				std::istringstream sstream(line);
				std::vector<ui32> col;
				while (sstream >> tileCode) {
					col.push_back(tileCode);
				}
				tileData.push_back(col);
			}
			if (tileData.size() > 0)
				init(tileData, levelWidth, levelHeight, levelDepth, nivel);
		}
	}

	// render level
	void Draw(SpriteRenderer* renderer) {
		for (GameObject* tile : this->Bricks)
		{
			if (!tile->Destroyed) {
				tile->Draw(renderer);
			}
		}
	}

	// check if the level is completed (all non-solid tiles are destroyed)
	bool IsCompleted() {
		for (GameObject* tile : this->Bricks)
		{
			if (!tile->Destroyed)
				return false;
		}
		return true;
	}

	i32 ActiveBall() {
		i32 cont = 0;
		for (GameObject* tile : this->Bricks)
		{
			if (!tile->Destroyed)
				cont++;
		}
		return cont;
	}

	BallObject* CrearBala() {	
		
		i32 cont = this->ActiveBall();
		cont = rand() % cont;

		i32 aux = 0;

		for (GameObject* tile : this->Bricks)
		{
			if (!tile->Destroyed) {
				aux++;

				if (aux == cont) {
					return new BallObject(tile->Position, 5.0f, -1.0f * vec3(0.0f, 30.0f, 0.0f),
						ResourceManager::GetTexture("face"));
				}
			}
		}
		return new BallObject();
	}

private:
	// initialize level from tile data
	void init(std::vector <std::vector<ui32>> tileData,
		ui32 levelWidth, ui32 levelHeight, ui32 levelDepth, ui32 nivel) {
		
		// calculate dimensions
		ui32 depth = tileData.size();
		ui32 width = tileData[0].size();
		ui32 height = 8;

		f32 unit_depth = levelDepth / static_cast<float>(depth);
		f32 unit_width = levelWidth / static_cast<float>(width);
		f32 unit_height = levelHeight / static_cast<float>(height);


		// initialize level tiles based on tileData
		ui32 y = height - nivel;
		for (ui32 z = 0; z < depth; ++z) {
			for (ui32 x = 0; x < width; ++x) {

				// check block type from level data (2D level array)
				
				if (tileData[z][x] > 1) {
					glm::vec3 color = glm::vec3(1.0f); // original white
					if (tileData[z][x] == 1)
						color = glm::vec3(0.2f, 0.6f, 1.0f);
					else if (tileData[z][x] == 2)
						color = glm::vec3(0.0f, 0.7f, 0.0f);
					else if (tileData[z][x] == 3)
						color = glm::vec3(0.8f, 0.8f, 0.4f);
					else if(tileData[z][x] == 4)
						color = glm::vec3(1.0f, 0.5f, 0.0f);

					glm::vec3 pos = vec3(1.0f);
					pos.x = unit_width * x + unit_width / 2.0f;
					pos.y = unit_height * y - unit_height / 2.0f;
					pos.z = unit_depth * z + unit_depth / 2.0f;

					glm::vec3 size(unit_width/2.0f, unit_height /2.0f, unit_depth/2.0f);;
					GameObject* obj = new GameObject(pos, size, ResourceManager::GetTexture("block"), color);
					Bricks.push_back(obj);
				}
			}
		}
	}
};

#endif 