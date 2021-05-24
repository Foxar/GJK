#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Renderer.h"
#include "Collider.h"

#include <iostream>

unsigned int Mesh::IDCounter = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
Renderer r;
bool firstMouse = true;
float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
bool rotate = true;

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    r.cameraFront = glm::normalize(front);
}
std::vector<Mesh *> vMeshList;
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        r.cameraPos += r.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        r.cameraPos -= r.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        r.cameraPos -= glm::normalize(glm::cross(r.cameraFront, r.cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        r.cameraPos += glm::normalize(glm::cross(r.cameraFront, r.cameraUp));

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        vMeshList[0]->m_pos.z += 1.1f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        vMeshList[0]->m_pos.z -= 1.1f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        vMeshList[0]->m_pos.x -= 1.1f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        vMeshList[0]->m_pos.x += 1.1f;

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        rotate = true;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        rotate = false;
}
int main()
{
    glfwSetCursorPosCallback(r.window, mouse_callback);

    //glUseProgram(shaderProgram);
    Mesh *box1 = new Mesh();
    Mesh *box2 = new Mesh();

    box1->Initialize();
    box2->Initialize();

    box1->m_pos = Vector<float>(0.0f, 0.0f, 0.0f);
    box2->m_pos = Vector<float>(6.0f, 100.0f, 0.0f);

    vMeshList.push_back(box1);
    vMeshList.push_back(box2);

    Collider col;

    while (!glfwWindowShouldClose(r.window))
    {
        box2->m_pos = Vector<float>(2.0f + (5 * sin(glfwGetTime())), 0.0f, 0.75f + 5 * cos(glfwGetTime()));
        box2->m_rot = Vector<float>((50 * sin(glfwGetTime())), glfwGetTime() * 100, 0);
        box1->m_rot = Vector<float>((50 * sin(glfwGetTime())), glfwGetTime() * 100, 0);
        processInput(r.window);
        for (int i = 0; i < vMeshList.size(); i++)
        {
            auto it1 = vMeshList.begin() + i;
            auto m1 = *it1;
            //m1->m_pos.y += -0.5f;
            m1->m_vel.y = -0.25f;
            if (m1->m_pos.y < 0.f)
                m1->Move(Vector<float>(0.0f, 0.0f - m1->m_pos.y, 0.0f));
            for (int j = i; j < vMeshList.size(); j++)
            {
                auto it2 = vMeshList.begin() + j;
                auto m2 = *it2;
                if (it1 == it2)
                    continue;

                if (col.GJK(m1, *it2))
                {
                    m1->SetColor(1.0f, 0.0f, 0.0f);
                    m1->debug_red = true;
                    m2->SetColor(1.0f, 0.0f, 0.0f);
                    m2->debug_red = true;
                }
                else
                {
                    if (m1->debug_red)
                        m1->ResetColor();
                    if (m2->debug_red)
                        m2->ResetColor();
                    m1->debug_red = false;
                    m2->debug_red = false;
                }
            }
            m1->Move(m1->m_vel);
            r.DrawMesh(m1);
        }
        r.Render();
    }
    glfwTerminate();
    return 0;
}
