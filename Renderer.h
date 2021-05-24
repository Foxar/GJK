#ifndef RENDERER_H
#define RENDERER_H
#include "Vector.h"
#include "Shader.h"
#include "Mesh.h"
// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
const float SCR_FOV = 70.0f;
class Renderer
{
public:
    //Shaders
    unsigned int vertexShader;
    unsigned int fragmentShader;

    Renderer()
    {
        printf("Building a renderer\n");
        InitializeWindow();
        shaderProgram = ShaderProgram("Shaders/vertexShader.vs", "Shaders/fragmentShader.vs");
        printf("Built renderer.\n");
    }
    ~Renderer()
    {
        for (int i = 0; i < vMeshList.size(); i++)
        {
            delete vMeshList[i];
        }
    }
    void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    void InitializeWindow()
    {
        printf("Initializing window...\n");
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        //glfwSetFramebufferSizeCallback(window, nullptr);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

        //Wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        printf("finished initializing\n");
    }

    unsigned VAO;
    unsigned VBO;
    Mesh m;

    ShaderProgram shaderProgram;
    GLFWwindow *window;

    void DrawMesh(Mesh *m)
    {
        vMeshList.push_back(m);
    }

    void Render()
    {
        shaderProgram.Use();

        //processInput(window);
        glClearColor(0.2f, 0.0f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderMeshes();

        glfwSwapBuffers(window);
        glfwPollEvents();
        vMeshList.clear();
    }

    void RenderMeshes()
    {
        for (int i = 0; i < vMeshList.size(); i++)
        {
            ModifyMatrices(vMeshList[i]);
            shaderProgram.Use();
            //Send color data to fragment shader using uniform data
            int vertexColorLocation = glGetUniformLocation(shaderProgram.ID, "fragmentColor");

            //glUseProgram(shaderProgram.ID);
            //glUniform4f(vertexColorLocation, 0.0f, 1.f, 1.0f, 1.0f);
            glUniform4f(vertexColorLocation, vMeshList[i]->color.x, vMeshList[i]->color.y, vMeshList[i]->color.z, 1.0f);

            glBindVertexArray(vMeshList[i]->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36); //VAOs

            //Use shader program
        }
    }

    void ModifyMatrices(Mesh *m)
    {
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        //Mesh position
        model = glm::translate(model, glm::vec3(m->m_pos.x, m->m_pos.y, m->m_pos.z));

        //X-Axis rotation
        model = glm::rotate(model, glm::radians(m->m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        //Y-Axis rotation
        model = glm::rotate(model, glm::radians(m->m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        //Z-Axis rotation
        model = glm::rotate(model, glm::radians(m->m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

        //Camera position and rotation
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        //view = glm::translate(view, glm::vec3(camera_pos.x, camera_pos.y, camera_pos.z));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        projection = glm::perspective(glm::radians(SCR_FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned modelUniformLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelUniformLoc, 1, GL_FALSE, glm::value_ptr(model));

        unsigned viewUniformLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewUniformLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned projUniformLoc = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projUniformLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }

    // camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

private:
    std::vector<Mesh *> vMeshList;
};
#endif