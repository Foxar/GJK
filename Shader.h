#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderProgram
{
public:
    unsigned int ID;

    ShaderProgram()
    {
    }
    ShaderProgram(const char *vertexPath, const char *fragmentPath)
    {
        std::string vertexShaderCode;
        std::string fragmentShaderCode;
        std::ifstream vertexShaderStream;
        std::ifstream fragmentShaderStream;
        printf("Creating a shader program...\n");
        vertexShaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vertexShaderStream.open(vertexPath);
            fragmentShaderStream.open(fragmentPath);
            std::stringstream vStringShaderStream, fStringShaderStream;

            vStringShaderStream << vertexShaderStream.rdbuf();
            fStringShaderStream << fragmentShaderStream.rdbuf();

            vertexShaderStream.close();
            fragmentShaderStream.close();

            vertexShaderCode = vStringShaderStream.str();
            fragmentShaderCode = fStringShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            printf("Shader error! Unable to load shader files.\n");
        }
        printf("Loaded shader code...\n");
        //Convert shader code from std::string to Cstring
        const char *vShaderCode_cstr = vertexShaderCode.c_str();
        const char *fShaderCode_cstr = fragmentShaderCode.c_str();

        unsigned int vertexID;
        unsigned int fragmentID;
        int success;
        char infoLog[512];
        printf("AA\n");

        //Compile vertex shader code
        vertexID = glCreateShader(GL_VERTEX_SHADER);
        printf("Created shader.\n");
        glShaderSource(vertexID, 1, &vShaderCode_cstr, NULL);
        glCompileShader(vertexID);
        //Print compile errors if any
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
            printf("Shader compilation error: %s\n", infoLog);
        };
        printf("Compiled vertex shader code!\n");
        //Compile vertex shader code
        fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentID, 1, &fShaderCode_cstr, NULL);
        glCompileShader(fragmentID);

        //Print compile errors if any
        glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
            printf("Shader compilation error: %s\n", infoLog);
        };
        printf("Compiled fragment shader code!\n");

        printf("Linking shader program...");
        //Create the shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertexID);
        glAttachShader(ID, fragmentID);
        glLinkProgram(ID);

        //Print program linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            printf("Shader program linking error: %s\n", infoLog);
        }

        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);
    }

    void Use()
    {
        glUseProgram(ID);
    }

    void setUniform(const std::string &name, bool v) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)v);
    }
    void setUniform(const std::string &name, int v) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)v);
    }
    void setUniform(const std::string &name, float v) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), v);
    }
};
#endif