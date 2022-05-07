#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <fstream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "object.hpp"
#include "sprite_renderer.hpp"
#include "resource_manager.hpp"

class level
{
public:
	std::vector<GameObject> Bricks;
	std::vector<std::vector<GLuint>> tileData;

	level() {}

	void load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	void updata();
	void draw(SpriteRenderer& renderer);

	GLboolean IsCompleted();

private:
	void init(std::vector<std::vector<GLuint>> tiles, GLuint levelWidth, GLuint levelHeight);
};

void level::load(const GLchar* file, GLuint levelWidth, GLuint levelHeight)
{
	this->Bricks.clear();
	//╪стьнд╪Ч
	GLuint tileCode;
	level Level;
	std::string line;
	std::ifstream in_file(file);

	if (in_file)
	{
		while (std::getline(in_file, line))
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}

		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
}

void level::draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : this->Bricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
}

GLboolean level::IsCompleted()
{
	for (GameObject& tile : this->Bricks)
		if (!tile.IsSolid && !tile.Destroyed)
			return GL_FALSE;
	return GL_TRUE;
}

void level::init(std::vector<std::vector<GLuint>> tiles, GLuint levelWidth, GLuint levelHeight)
{
	GLuint height = tiles.size();
	GLuint width = tiles[0].size();
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width), unit_height = levelHeight / height;

	for (GLuint y = 0; y < height; y++)
	{
		for (GLuint x = 0; x < width; x++)
		{
			if (tiles[y][x] == 1)
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = GL_TRUE;
				obj.Value = 1;
				this->Bricks.push_back(obj);
			}
			else if (tiles[y][x] > 1)	// Non-solid; now determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				Texture2D texture;
				GLuint value = 0;
				if (tiles[y][x] == 2)
				{
					//color = glm::vec3(0.2f, 0.6f, 1.0f);
					texture = ResourceManager::GetTexture("blue1");
					value = 2;
				}
				else if (tiles[y][x] == 3)
				{
					//color = glm::vec3(0.0f, 0.7f, 0.0f);
					texture = ResourceManager::GetTexture("red1");
					value = 3;
				}
				else if (tiles[y][x] == 4)
				{
					//color = glm::vec3(0.8f, 0.8f, 0.4f);
					texture = ResourceManager::GetTexture("yellow1");
					value = 4;
				}
				else if (tiles[y][x] == 5)
				{
					//color = glm::vec3(1.0f, 0.5f, 0.0f);
					texture = ResourceManager::GetTexture("purple1");
					value = 5;
				}

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				this->Bricks.push_back(GameObject(pos, size, texture, color,glm::vec2(0.0f),1,value));
			}
		}
	}
}

void level::updata()
{
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	Texture2D texture;

	for (GLuint y = 0; y < height; y++)
	{
		for (GLuint x = 0; x < width; x++)
		{
			if (tileData[y][x] == 2 && Bricks[y * width + x].HP ==0)
			{
				//color = glm::vec3(0.2f, 0.6f, 1.0f);
				texture = ResourceManager::GetTexture("blue2");
				Bricks[y * width + x] = GameObject(Bricks[y * width + x].Pos, Bricks[y * width + x].Size, texture, glm::vec3(1.0f), glm::vec2(0.0f), 0);
				std::cout << Bricks.size()<< "      " << y * width + x << std::endl;
			}
			else if (tileData[y][x] == 3 && Bricks[y * width + x].HP == 0)
			{
				//color = glm::vec3(0.0f, 0.7f, 0.0f);
				texture = ResourceManager::GetTexture("red2");
				Bricks[y * width + x] = GameObject(Bricks[y * width + x].Pos, Bricks[y * width + x].Size, texture, glm::vec3(1.0f), glm::vec2(0.0f), 0);
			}
			else if (tileData[y][x] == 4 && Bricks[y * width + x].HP == 0)
			{
				//color = glm::vec3(0.8f, 0.8f, 0.4f);
				texture = ResourceManager::GetTexture("yellow2");
				Bricks[y * width + x] = GameObject(Bricks[y * width + x].Pos, Bricks[y * width + x].Size, texture, glm::vec3(1.0f), glm::vec2(0.0f), 0);

			}
			else if (tileData[y][x] == 5 && Bricks[y * width + x].HP == 0)
			{
				//color = glm::vec3(1.0f, 0.5f, 0.0f);
				texture = ResourceManager::GetTexture("purple2");
				Bricks[y * width + x] = GameObject(Bricks[y * width + x].Pos, Bricks[y * width + x].Size, texture, glm::vec3(1.0f), glm::vec2(0.0f), 0);
			}
		}
	}
}

#endif // !LEVEL_HPP
