//
// Created by Roger on 30/10/2024.
//

#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>


class Shader {
public:
    static std::vector<std::shared_ptr<Shader>> shaders;
    
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    ~Shader();
    
    void loadShader(const char* vertexPath, const char* fragmentPath);
    void loadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

    void SetMatrix(const char* name, glm::mat4 matrix) const;

private:
    
    GLuint CompileShader(const char* shaderPath, GLenum shaderType, const char* typeStr);
    void CompileShader(GLuint& shaderID, const char* filename, GLenum shaderType);
public:
    
    GLuint shaderProgram;
};



#endif //SHADER_H
