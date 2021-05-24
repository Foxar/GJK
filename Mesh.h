#ifndef MESH_H
#define MESH_H
#include <vector>
#include <math.h>
#include "ModelLoader.h"
#include "Vector.h"

#define degreesToRadians(x) x *(3.141592f / 180.0f)

//Only solid colored meshes at the time
class Mesh
{
public:
    unsigned int id;
    static unsigned int IDCounter;
    unsigned int collidingWithID = -1;
    bool debug_red = false;
    Mesh()
    {
        id = IDCounter++;
    }
    void Initialize()
    {
        printf("Initializing model\n");
        ModelLoader::LoadFromObj("Model.obj", m_vertices);

        //Create a VBO so we can send vertices to a VBO for OpenGL to use
        glGenBuffers(1, &VBO);

        //Creating a VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //Bind VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vector<float>), &m_vertices[0], GL_STATIC_DRAW);

        //Link vertex attributes
        //Position attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0); //Stride is 3 * sizeof(float) as there's three position floats
        glEnableVertexAttribArray(0);

        float r = fmod(rand(), 100.0f) / 100.0f;
        float g = fmod(rand(), 100.0f) / 100.0f;
        float b = fmod(rand(), 100.0f) / 100.0f;

        color = Vector<float>(r, g, b);
    }
    void SetColor(float r, float g, float b)
    {
        color.x = r;
        color.y = g;
        color.z = b;
    }
    void ResetColor()
    {
        float r = fmod(rand(), 100.0f) / 100.0f;
        float g = fmod(rand(), 100.0f) / 100.0f;
        float b = fmod(rand(), 100.0f) / 100.0f;

        debug_red = false;

        color = Vector<float>(r, g, b);
    }
    void LoadVertices(const std::vector<float> &_vertices)
    {
        std::vector<Vector<float>> vectorList;
        for (int i = 0; i < _vertices.size(); i += 3)
        {
            vectorList.push_back(Vector<float>(_vertices[i], _vertices[i + 1], _vertices[i + 2]));
        }
        m_vertices = vectorList;
    }

    std::vector<Vector<float>> GetRotatedVertices()
    {
        std::vector<Vector<float>> rotatedVertices = m_vertices;
        Vector<float> radianRot = m_rot;
        radianRot.x = degreesToRadians(m_rot.x);
        radianRot.y = degreesToRadians(m_rot.y);
        radianRot.z = degreesToRadians(m_rot.z);

        for (int i = 0; i < rotatedVertices.size(); i++)
        {
            //X-Axis
            rotatedVertices[i].y = (cos(radianRot.x) * rotatedVertices[i].y) - (sin(radianRot.x) * rotatedVertices[i].z);
            rotatedVertices[i].z = (sin(radianRot.x) * rotatedVertices[i].y) - (cos(radianRot.x) * rotatedVertices[i].z);

            //Y-Axis
            rotatedVertices[i].x = (cos(radianRot.y) * rotatedVertices[i].x) + (sin(radianRot.y) * rotatedVertices[i].z);
            rotatedVertices[i].z = (sin(radianRot.y) * rotatedVertices[i].x) + (cos(radianRot.y) * rotatedVertices[i].z);

            //Z-Axis
            rotatedVertices[i].x = (cos(radianRot.z) * rotatedVertices[i].x) - (sin(radianRot.z) * rotatedVertices[i].y);
            rotatedVertices[i].y = (sin(radianRot.z) * rotatedVertices[i].x) - (cos(radianRot.z) * rotatedVertices[i].y);
        }
        return rotatedVertices;
    }

    void Rotate(Vector<float> _rot)
    {
        m_rot += _rot;
        m_rot.x = fmod(m_rot.x, 360.0f);
        m_rot.y = fmod(m_rot.y, 360.0f);
        m_rot.z = fmod(m_rot.z, 360.0f);
    }

    void Move(Vector<float> _move)
    {
        m_pos += _move;
    }

    unsigned VAO, VBO, EBO;
    Vector<float> color; //RGB
    std::vector<Vector<float>> m_vertices;
    Vector<float> m_pos;
    Vector<float> m_vel;
    Vector<float> m_rot;
};
#endif