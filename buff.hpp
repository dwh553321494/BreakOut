#ifndef BUFF_HPP
#define BUFF_HPP

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "object.hpp"

const glm::vec2 SIZE(150, 70);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class Buff :public GameObject
{
public:
    // 道具类型
    std::string Type;
    GLfloat     Duration;
    GLboolean   Activated;
    // 构造函数
    Buff(std::string type, glm::vec3 color, GLfloat duration,
        glm::vec2 position, Texture2D texture)
        : GameObject(position, SIZE, texture, color, VELOCITY),
        Type(type), Duration(duration), Activated()
    { }
};

#endif // BUFF_HPP
