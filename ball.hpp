#ifndef BALL_HPP
#define BALL_HPP

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "sprite_renderer.hpp"
#include "object.hpp"



class Ball :public GameObject
{
public:	
    GLfloat   Radius;
    GLboolean Stuck;

    Ball() :GameObject(), Radius(12.5f), Stuck(true) {};
    Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
        :GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) {};

    // Moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
    glm::vec2 move(GLfloat dt, GLuint window_width);
    // Resets the ball to original state with given position and velocity
    void reset(glm::vec2 position, glm::vec2 velocity);
};


glm::vec2 Ball::move(GLfloat dt, GLuint window_width)
{
    // If not stuck to player board
    if (!this->Stuck)
    {
        // Move the ball
        this->Pos += this->Velocity * dt;
        // Then check if outside window bounds and if so, reverse velocity and restore at correct position
        if (this->Pos.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Pos.x = 0.0f;
        }
        else if (this->Pos.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Pos.x = window_width - this->Size.x;
        }
        if (this->Pos.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Pos.y = 0.0f;
        }
    }
    return this->Pos;
}

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void Ball::reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Pos = position;
    this->Velocity = velocity;
    this->Stuck = true;
}

#endif // !BALL_HPP

