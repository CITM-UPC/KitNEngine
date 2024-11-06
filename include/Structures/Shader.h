//
// Created by Roger on 30/10/2024.
//

#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL\glew.h>


class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    
    
    ~Shader();
    void loadShader(const char* vertexPath, const char* fragmentPath);
    void loadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

    GLuint CompileShader(const char* shaderPath, GLenum shaderType, const char* typeStr);
    void CompileShader(GLuint& shaderID, const char* filename, GLenum shaderType);
public:
    
    GLuint shaderProgram;
};



#endif //SHADER_H
