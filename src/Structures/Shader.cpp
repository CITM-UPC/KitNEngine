//
// Created by Roger on 30/10/2024.
//

#include "Structures/Shader.h"

#include "Utilities/FileUtils.h"
#include <iostream>

std::vector<std::shared_ptr<Shader>> Shader::shaders = std::vector<std::shared_ptr<Shader>>();

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    if (geometryPath != nullptr)
        loadShader(vertexPath, fragmentPath, geometryPath);
    else
        loadShader(vertexPath, fragmentPath);
}

Shader::~Shader()
{
    glDeleteProgram(shaderProgram);
}

void Shader::loadShader(const char* vertexPath, const char* fragmentPath)
{
    shaderProgram = glCreateProgram();
    GLuint vertexShader;
    GLuint fragmentShader;
    this->CompileShader(vertexShader,vertexPath,GL_VERTEX_SHADER);
    this->CompileShader(fragmentShader,fragmentPath,GL_FRAGMENT_SHADER);
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Set the 2D sampler for texture rendering
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "Sampler"), 0);
    glUseProgram(0);

    // Check for linking errors
    GLint success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::loadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    std::cerr << "Geometry shaders not yet implemented!" << std::endl;
}

void Shader::SetViewMatrix(glm::mat4 matrix) const
{
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"view"), 1, GL_FALSE, &matrix[0][0]);
}

GLuint Shader::CompileShader(const char* shaderPath, GLenum shaderType, const char* typeStr)
{
    std::string source = LoadTextFile(shaderPath);
    const GLchar* str = source.c_str();
    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &str, NULL);
    glCompileShader(shaderID);

    GLint  success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << typeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shaderID;
}

void Shader::CompileShader(GLuint& shaderID, const char* filename, GLenum shaderType)
{
    std::string source = LoadTextFile(filename);
    const GLchar* str = source.c_str();
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &str, NULL);
    glCompileShader(shaderID);

    GLint  success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::string shaderTypeString;
        switch(shaderType)
        {
        case GL_VERTEX_SHADER: shaderTypeString = "VERTEX"; break;
        case GL_FRAGMENT_SHADER: shaderTypeString = "FRAGMENT"; break;
        default: shaderTypeString = "UNKNOWN"; break;
        }
        std::cout << "ERROR::SHADER::" << shaderTypeString << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


}
