#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

#include "game.hpp"
#include "resource_manager.hpp"


const char* GameLevels[4]{ "1", "2", "3", "4"};

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const GLuint SCR_WIDTH = 2000;
// The height of the screen
const GLuint SCR_HEIGHT = 1200;

Game Breakout(SCR_WIDTH, SCR_HEIGHT);

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BreakOut", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //this line associates the function framebuffer_size_callback with the resize event

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // Setup Dear ImGui style
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();


    glfwSetKeyCallback(window, key_callback);

    // OpenGL configuration
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize game
    Breakout.Init();

    // DeltaTime variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Start Game within Menu State
    Breakout.State = GAME_ACTIVE;
    
    // Gui参数
    bool GameGui = true;
    bool show_demo_window = true;
    bool show_rule_window = false;
    bool show_gui_setup = false;

    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // 创建GUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("BreakOut", &GameGui, ImGuiWindowFlags_MenuBar);

        // UI功能
        ImGui::Checkbox("Demo Window", &show_demo_window); ImGui::SameLine();
        ImGui::Checkbox("rules", &show_rule_window); ImGui::SameLine();
        ImGui::Checkbox("gui SetUp", &show_gui_setup);
        ImGui::Combo("Level", &Breakout.Level, GameLevels, IM_ARRAYSIZE(GameLevels), 4);
        

        ImGui::Text("your current score:"); ImGui::SameLine();
        ImGui::InputInt("Score", &Score);
        

        ImGui::End();

        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(800, 0), ImGuiCond_FirstUseEver);
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        if (show_gui_setup)
        {
            ImGui::Begin("gui set up", &show_gui_setup);
            ImGui::ShowStyleEditor();
            ImGui::End();
        }
        if (show_rule_window)
        {
            ImGui::Begin("Rules", &show_rule_window);
            ImGui::Text("This is a classic BreakOut mini-game, including 4 different levels, you can control the cursor to catch the ball");
            ImGui::Button("A"); ImGui::SameLine(); ImGui::Text("Left"); ImGui::SameLine();
            ImGui::Button("D"); ImGui::SameLine(); ImGui::Text("Right"); ImGui::SameLine();
            ImGui::Button("blank space"); ImGui::SameLine(); ImGui::Text("Start");
            ImGui::Text("+100 : Score +100"); ImGui::SameLine(); ImGui::SliderInt("Probability1:1/x", &Probability[0], 1, 100); ImGui::ColorEdit3("color1", (float*)&ColorEdit[0]);
            ImGui::Text("+250 : Score +250"); ImGui::SameLine(); ImGui::SliderInt("Probability2:1/x", &Probability[1], 1, 100); ImGui::ColorEdit3("color2", (float*)&ColorEdit[1]);
            ImGui::Text("+500 : Score +500"); ImGui::SameLine(); ImGui::SliderInt("Probability3:1/x", &Probability[2], 1, 100); ImGui::ColorEdit3("color3", (float*)&ColorEdit[2]);
            ImGui::Text("Slow : The speed of the ball down"); ImGui::SliderFloat("Lower multiples", &Lower, 0.5f, 1.0f); ImGui::SliderInt("Probability4:1/x", &Probability[3], 1, 100); ImGui::ColorEdit3("color4", (float*)&ColorEdit[4]);
            ImGui::Text("Fast : The speed of the ball increase"); ImGui::SliderFloat("Increase multiples", &Increase, 1.0f, 1.5f); ImGui::SliderInt("Probability5:1/x", &Probability[4], 1, 100); ImGui::ColorEdit3("color5", (float*)&ColorEdit[5]);
            ImGui::Text("<- -> : The cursor to expand"); ImGui::SameLine(); ImGui::SliderInt("Probability6:1/x", &Probability[5], 1, 100); ImGui::ColorEdit3("color6", (float*)&ColorEdit[5]);
            ImGui::Text("-> <- : The cursor to narrow"); ImGui::SameLine(); ImGui::SliderInt("Probability7:1/x", &Probability[6], 1, 100); ImGui::ColorEdit3("color7", (float*)&ColorEdit[6]);
            
            //ImGui::SliderInt("Particles mount", &ParticlesMount, 500, 1000);
            ImGui::SliderInt("LIves", &Breakout.Lives, 1, 100);
            
            ImGui::End();
        }

        //deltaTime = 0.001f;
        // Manage user input
        Breakout.ProcessInput(deltaTime);

        // Update Game state
        Breakout.Update(deltaTime);

        // Render
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Delete all resources as loaded using the resource manager
    ResourceManager::Clear();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
        {
            Breakout.Keys[key] = GL_FALSE;
            Breakout.KeysProcessed[key] = GL_FALSE;
        }
    }
}