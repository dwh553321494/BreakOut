#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "TextRenderer.hpp"
#include "particle.hpp"
#include "level.hpp"
#include "ball.hpp"
#include "buff.hpp"

enum GameState{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// 碰撞检查
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;
Collision CheckCollision(Ball& ball, GameObject& rec);
GLboolean CheckCollision(GameObject& one, GameObject& two);

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // 上
        glm::vec2(1.0f, 0.0f),  // 右
        glm::vec2(0.0f, -1.0f), // 下
        glm::vec2(-1.0f, 0.0f)  // 左
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

// 道具加成
GLboolean ShouldCreateBuff(GLuint chance) {
    GLuint random = rand() % chance;
    return random == 0;
};
void ActivateBuff(Buff& buff);


// Game-related State data
SpriteRenderer*              Renderer;
TextRenderer*                Text;
ParticleGenerator*           Particles;
GameObject*                  Player;
Ball*                        ball;
int                          Score;

std::vector<int>             Probability{ 2, 5, 8, 20, 20, 15, 15 };
std::vector<ImVec4>          ColorEdit{
                                        ImVec4(1.0f,1.0f,1.0f,1.0f),
                                        ImVec4(1.0f,1.0f,1.0f,1.0f),
                                        ImVec4(1.0f,1.0f,1.0f,1.0f),
                                        ImVec4(1.0f,1.0f,1.0f,1.0f),
                                        ImVec4(1.0f,1.0f,1.0f,1.0f),
                                        ImVec4(1.0f,1.0f,1.0f,1.0f),
                                        ImVec4(1.0f,1.0f,1.0f,1.0f)};
std::vector <glm::vec3>      BuffsColor{ 
                                            {ColorEdit[0].x,ColorEdit[0].y,ColorEdit[0].z},
                                            {ColorEdit[1].x,ColorEdit[1].y,ColorEdit[1].z},
                                            {ColorEdit[2].x,ColorEdit[2].y,ColorEdit[2].z},
                                            {ColorEdit[3].x,ColorEdit[3].y,ColorEdit[3].z},
                                            {ColorEdit[4].x,ColorEdit[4].y,ColorEdit[4].z},
                                            {ColorEdit[5].x,ColorEdit[5].y,ColorEdit[5].z},
                                            {ColorEdit[6].x,ColorEdit[6].y,ColorEdit[6].z} };
float                        Lower = 0.75f;
float                        Increase = 1.2f;

// 游标大小和速度
const glm::vec2 PLAYER_SIZE(150, 30);
const GLfloat PLAYER_SPEED(500.f);
// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 25.0f;

int ParticlesMount = 500;


class Game
{
public:
    // Game state
    GameState              State;
    GLboolean              Keys[1024];
    GLboolean              KeysProcessed[1024];
    GLuint                 Width, Height;

    int                    Lives = 3;
    int                    Level;
    std::vector<level>     Levels;

    std::vector<Buff>      Buffs;


    Game(GLuint width, GLuint height);
    ~Game();

    // Initialize game state (load all shaders/textures/levels)
    void Init();

    void ResetLevel();
    void ResetPlayer();

    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();

    // function
    void Collisions();
    void CreateBuffs(GameObject& block);
    void UpdateBuffs(GLfloat dt);
};

Game::Game(GLuint width, GLuint height)
    : State(GAME_MENU), Keys(), Width(width), Height(height)
{}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("sprite.vs.txt", "sprite.fs.txt", nullptr, "sprite");
    ResourceManager::LoadShader("particle.vs.txt", "particle.fs.txt", nullptr, "particle");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);
    // Load textures
    // 背景和球
    ResourceManager::LoadTexture("img/paddle.png", GL_TRUE, "paddle");
    ResourceManager::LoadTexture("img/parallax-mountain.png", GL_FALSE, "background");
    ResourceManager::LoadTexture("img/ball.png", GL_TRUE, "ball");
    ResourceManager::LoadTexture("img/FlameParticle1_I.jpg", GL_FALSE, "particle");

    // 方块纹理
    ResourceManager::LoadTexture("img/01-blue-Tiles.png", GL_TRUE, "blue1");
    ResourceManager::LoadTexture("img/02-blue-Tiles.png", GL_TRUE, "blue2");
    ResourceManager::LoadTexture("img/03-green-Tiles.png", GL_TRUE, "green1");
    ResourceManager::LoadTexture("img/04-green-Tiles.png", GL_TRUE, "green2");
    ResourceManager::LoadTexture("img/05-purple-Tiles.png", GL_TRUE, "purple1");
    ResourceManager::LoadTexture("img/06-purple-Tiles.png", GL_TRUE, "purple2");
    ResourceManager::LoadTexture("img/07-red-Tiles.png", GL_TRUE, "red1");
    ResourceManager::LoadTexture("img/08-red-Tiles.png", GL_TRUE, "red2");
    ResourceManager::LoadTexture("img/09-yellow-Tiles.png", GL_TRUE, "yellow1");
    ResourceManager::LoadTexture("img/10-yellow-Tiles.png", GL_TRUE, "yellow2");

    ResourceManager::LoadTexture("img/34-100-Tiles.png", GL_TRUE, "100");
    ResourceManager::LoadTexture("img/39-250-Tiles.png", GL_TRUE, "250");
    ResourceManager::LoadTexture("img/40-500-Tiles.png", GL_TRUE, "500");

    ResourceManager::LoadTexture("img/41-slow-Tiles.png", GL_TRUE, "slow");
    ResourceManager::LoadTexture("img/42-fast-Tiles.png", GL_TRUE, "fast");
    ResourceManager::LoadTexture("img/46-short-Tiles.png", GL_TRUE, "short");
    ResourceManager::LoadTexture("img/47-long-Tiles.png", GL_TRUE, "long");

    ResourceManager::LoadTexture("img/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("img/block_solid.png", GL_FALSE, "block_solid");

    // Set render-specific controls
    Renderer = new SpriteRenderer((Shader&)ResourceManager::GetShader("sprite"));
    // Load levels
    level one; one.load("levels/one.txt", this->Width, this->Height * 0.5);
    level two; two.load("levels/two.txt", this->Width, this->Height * 0.5);
    level three; three.load("levels/three.txt", this->Width, this->Height * 0.5);
    level four; four.load("levels/four.txt", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // Configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);

    ball = new Ball(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));

    Particles = new ParticleGenerator(
        ResourceManager::GetShader("particle"),
        ResourceManager::GetTexture("particle"),
        ParticlesMount
    );

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/ocraext.TTF", 24);
}

void Game::ResetLevel()
{
    //if (this->Level == 0)this->Levels[0].load("levels/one.txt", this->Width, this->Height * 0.5f);
    //else if (this->Level == 1)
    //    this->Levels[1].load("levels/two.txt", this->Width, this->Height * 0.5f);
    //else if (this->Level == 2)
    //    this->Levels[2].load("levels/three.txt", this->Width, this->Height * 0.5f);
    //else if (this->Level == 3)
    //    this->Levels[3].load("levels/four.txt", this->Width, this->Height * 0.5f);
    this->Levels.clear();
    level one; one.load("levels/one.txt", this->Width, this->Height * 0.5);
    level two; two.load("levels/two.txt", this->Width, this->Height * 0.5);
    level three; three.load("levels/three.txt", this->Width, this->Height * 0.5);
    level four; four.load("levels/four.txt", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Lives = 3;
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Pos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    ball->reset(Player->Pos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
    // Also disable all active powerups
    Player->Color = glm::vec3(1.0f);
    ball->Color = glm::vec3(1.0f);
}

void Game::Update(GLfloat dt)
{
    Particles->Update(dt, *ball, 20, glm::vec2(ball->Radius / 2));

    ball->move(dt, this->Width);
    this->Collisions();
    //this->Levels[Level].updata();
    this->UpdateBuffs(dt);


    if (ball->Pos.y >= this->Height)
    {
        --this->Lives;
        if (this->Lives == 0)
        {
            Score = 0;
            this->ResetLevel();
            this->State = GAME_MENU;
        }
        this->ResetPlayer();
    }

    if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
    {
        this->Level++;
        this->ResetLevel();
        this->ResetPlayer();
        this->State = GAME_WIN;
    }

}


void Game::ProcessInput(GLfloat dt)
{
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_SPEED * dt;
        // 移动玩家挡板
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Pos.x >= 0)
            {
                Player->Pos.x -= velocity;
                if (ball->Stuck)
                    ball->Pos.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Pos.x <= this->Width - Player->Size.x)
            {
                Player->Pos.x += velocity;
                if (ball->Stuck)
                    ball->Pos.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
            ball->Stuck = false;
    }
    if (this->State == GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            this->State = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
        }
    }
    if (this->State == GAME_WIN)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
            this->State = GAME_MENU;
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU)
    {
        // Draw background
        Renderer->DrawSprite((Texture2D&)(ResourceManager::GetTexture("background")), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
        // Draw level
        this->Levels[this->Level].draw(*Renderer);
        // Draw player
        Player->Draw(*Renderer);
        ball->Draw(*Renderer);
        Particles->Draw();

        for (Buff& buff : this->Buffs)
        {
            if (!buff.Destroyed)
                buff.Draw(*Renderer);
        }

        std::stringstream ss_level; ss_level << this->Level;
        Text->RenderText("Level:" + ss_level.str(), 5.0f, 5.0f, 1.0f);
        std::stringstream ss_lives; ss_lives << this->Lives;
        Text->RenderText("Lives:" + ss_lives.str(), 1500.0f, 5.0f, 1.0f);
    }
    if (this->State == GAME_MENU)
    {
        Text->RenderText("Press ENTER to Strat", 500.0f, Height / 2, 1.0f);
    }
    if (this->State == GAME_WIN)
    {
        std::stringstream ss_Score; ss_Score << Score;
        Text->RenderText(
            "YOU WIN,your score is" + ss_Score.str(), 620, Height / 2, 1.5, glm::vec3(1.0f, 0.1f, 0.1f));
        Text->RenderText(
            "Press ENTER to next level", 750, Height / 2, 1.0f, glm::vec3(1.0f));
        Score = 0;
    }
}

void Game::Collisions()
{
    for (GameObject& box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*ball, box);
            if (std::get<0>(collision)) // 如果collision 是 true
            {
                // 如果砖块不是实心就销毁砖块
                if ((!box.IsSolid) && box.HP == 0)
                {
                    box.Destroyed = GL_TRUE;
                    this->CreateBuffs(box);
                }
                // 碰撞处理
                else if (box.Value == 2)
                {
                    box = GameObject(box.Pos, box.Size, ResourceManager::GetTexture("blue2"), glm::vec3(1.0f), glm::vec2(0.0f), 0, 2);
                }
                else if (box.Value == 3)
                {
                    box = GameObject(box.Pos, box.Size, ResourceManager::GetTexture("red2"), glm::vec3(1.0f), glm::vec2(0.0f), 0, 2);
                }
                else if (box.Value == 4)
                {
                    box = GameObject(box.Pos, box.Size, ResourceManager::GetTexture("yellow2"), glm::vec3(1.0f), glm::vec2(0.0f), 0, 2);
                }
                else if (box.Value == 5)
                {
                    box = GameObject(box.Pos, box.Size, ResourceManager::GetTexture("purple2"), glm::vec3(1.0f), glm::vec2(0.0f), 0, 2);
                }


                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) // 水平方向碰撞
                {
                    ball->Velocity.x = -ball->Velocity.x; // 反转水平速度
                    // 重定位
                    GLfloat penetration = ball->Radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        ball->Pos.x += penetration; // 将球右移
                    else
                        ball->Pos.x -= penetration; // 将球左移
                }
                else // 垂直方向碰撞
                {
                    ball->Velocity.y = -ball->Velocity.y; // 反转垂直速度
                    //ball->Velocity.y = -1 * abs(ball->Velocity.y);
                    // 重定位
                    GLfloat penetration = ball->Radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        ball->Pos.y -= penetration; // 将球上移
                    else
                        ball->Pos.y += penetration; // 将球下移
                }
            }
        }
    }

    Collision result = CheckCollision(*ball, *Player);
    if (!ball->Stuck && std::get<0>(result))
    {
        // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
        GLfloat centerBoard = Player->Pos.x + Player->Size.x / 2;
        GLfloat distance = (ball->Pos.x + ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2);
        // 依据结果移动
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = ball->Velocity;
        ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->Velocity.y = -1 * abs(ball->Velocity.y);
        ball->Velocity = glm::normalize(ball->Velocity) * glm::length(oldVelocity);
    }

    for (Buff& buff : this->Buffs)
    {
        if (!buff.Pos.y >= this->Height)
            buff.Destroyed = GL_TRUE;
        if (CheckCollision(*Player, buff))
        {
            ActivateBuff(buff);
            buff.Destroyed = GL_TRUE;
            buff.Activated = GL_TRUE;
        }
    }
}

void Game::CreateBuffs(GameObject& block)
{
    if (ShouldCreateBuff(Probability[0])) 
        this->Buffs.push_back(
            Buff("100", BuffsColor[0], 0.0f, block.Pos, ResourceManager::GetTexture("100")
        )); 
    else if(ShouldCreateBuff(Probability[1])) 
        this->Buffs.push_back(
            Buff("250", BuffsColor[1], 0.0f, block.Pos, ResourceManager::GetTexture("250")
            ));
    else if(ShouldCreateBuff(Probability[2])) 
        this->Buffs.push_back(
            Buff("500", BuffsColor[2], 0.0f, block.Pos, ResourceManager::GetTexture("500")
            ));

    if (ShouldCreateBuff(Probability[3]))
        this->Buffs.push_back(
            Buff("slow", BuffsColor[3], 0.0f, block.Pos, ResourceManager::GetTexture("slow"))
        );
    else if (ShouldCreateBuff(Probability[4]))
        this->Buffs.push_back(
            Buff("fast", BuffsColor[4], 0.0f, block.Pos, ResourceManager::GetTexture("fast"))
        );

    if (ShouldCreateBuff(Probability[5]))
        this->Buffs.push_back(
            Buff("short", BuffsColor[5], 0.0f, block.Pos, ResourceManager::GetTexture("short"))
        );
    if (ShouldCreateBuff(Probability[6]))
        this->Buffs.push_back(
            Buff("long", BuffsColor[6], 0.0f, block.Pos, ResourceManager::GetTexture("long"))
        );

}

void Game::UpdateBuffs(GLfloat dt)
{
    for (Buff& buff : this->Buffs)
    {
        buff.Pos += buff.Velocity * dt;
        if (buff.Activated)
        {
            buff.Duration -= dt;

            if (buff.Duration <= 0.0f)
            {
                buff.Activated = GL_FALSE;

                //if (buff.Type == "slow")
                //{
                //    ball->Velocity *= 2.0f;
                //}
            }
        }
    }
    this->Buffs.erase(std::remove_if(this->Buffs.begin(), this->Buffs.end(),
        [](const Buff& buff) {return buff.Destroyed && !buff.Activated; }
    ), this->Buffs.end());
}


Collision CheckCollision(Ball& ball, GameObject& rec)
{
    // 获取球的中心
    glm::vec2 center(ball.Pos + ball.Radius);
    // 获取矩形的中心和半边长
    glm::vec2 rec_half(rec.Size.x / 2, rec.Size.y / 2);
    glm::vec2 rec_center(
        rec.Pos.x + rec_half.x,
        rec.Pos.y + rec_half.y
    );

    // 获取中心矢量差
    glm::vec2 dcenter = center - rec_center;
    glm::vec2 clamped = glm::clamp(dcenter, -rec_half, rec_half);
    glm::vec2 closest = rec_center + clamped;

    dcenter = closest - center;
    if (glm::length(dcenter) <= ball.Radius)
        return std::make_tuple(GL_TRUE, VectorDirection(dcenter), dcenter);
    else
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0.0f, 0.0f));
}
GLboolean CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // Collision x-axis?
    GLboolean collisionX = one.Pos.x + one.Size.x >= two.Pos.x &&
        two.Pos.x + two.Size.x >= one.Pos.x;
    // Collision y-axis?
    GLboolean collisionY = one.Pos.y + one.Size.y >= two.Pos.y &&
        two.Pos.y + two.Size.y >= one.Pos.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}

void ActivateBuff(Buff& buff)
{
    if (buff.Type == "100")
    {
        Score += 100;
    }
    else if (buff.Type == "250")
    {
        Score += 250;
    }
    else if (buff.Type == "500")
    {
        Score += 500;
    }
    else if (buff.Type == "slow")
    {
        ball->Velocity *= Lower;
    }
    else if (buff.Type == "fast")
    {
        ball->Velocity *= Increase;
    }
    else if(buff.Type == "short")
    {
        Player->Size.x *= 0.75f;
    }
    else if (buff.Type == "long")
    {
        Player->Size.x *= 1.5f;
    }

}

#endif // !GAME_HPP