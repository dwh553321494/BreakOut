#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "sprite_renderer.hpp"

class GameObject
{
public:
	glm::vec2 Pos, Size, Velocity;
	glm::vec3 Color;
	GLfloat Rotation;
	GLboolean IsSolid;
	GLboolean Destroyed;
	GLint Value;
	GLint HP;

	Texture2D Sprite;

	GameObject() : Pos(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false), HP(1), Value(0) {};

	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f), GLint hp = 1,GLint value = 0)
		: Pos(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false), HP(hp), Value(value) { };

	// Draw sprite
	virtual void Draw(SpriteRenderer& renderer);

};

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(this->Sprite, this->Pos, this->Size, this->Rotation, this->Color);
}

#endif // OBJECT_HPP