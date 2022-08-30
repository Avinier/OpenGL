#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include<fstream>
#include<string>
#include<sstream>

#include"Renderer.h"
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include"VertexBufferLayout.h"
#include"VertexArray.h"
#include"Shader.h"
#include"Texture.h"

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw_gl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Sycing the framerate
    glfwSwapInterval(1);

  
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float position[] = 
    {
       -0.5f,  -0.5f, 0.0f, 0.0f, //0
        0.5f,  -0.5f, 1.0f, 0.0f, //1
        0.5f,   0.5f, 1.0f, 1.0f, //2
       -0.5f,   0.5f, 0.0f, 1.0f  //3

    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    //Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Creating vertex array object which will hold the vertex buffers
    //Creating a buffer which stores the data for the shapes
    VertexArray va;
    VertexBuffer vb(position, 4 * 4 * sizeof(float));


    //connects buffer and vao
    VertexBufferLayout layout;

    layout.Push<float>(2);
    layout.Push<float>(2);

    va.AddBuffer(vb, layout);


    //Creating an index buffer
    IndexBuffer ib(indices, 6);
    
    //Projection Matrices
    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    
   
    //GOTO: line 14
    
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.5f, 0.3f, 0.8f, 1.0f);

    //Textures
    Texture texture("res/textures/berserk1.png");
    texture.Bind();

    shader.SetUniform1i("u_Texture", 0);

    //Unbinding all the buffers
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    //Imgui framework
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsClassic();

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    float r = 0.0f, increment = 0.05f;
    glm::vec3 translation(0.0f, 0.0f, 0.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        ImGui_ImplGlfwGL3_NewFrame();

        //Moving the the object irt
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = proj * view * model;

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);


        renderer.Draw(va, ib, shader);

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        {
            ImGui::SliderFloat("Translation", &translation.x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::SliderFloat("Translation y", &translation.y, 0.0f, 1.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

       

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}