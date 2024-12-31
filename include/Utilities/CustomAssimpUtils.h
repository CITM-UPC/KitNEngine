//
// Created by Roger on 31/12/2024.
//

#ifndef CUSTOMASSIMPUTILS_H
#define CUSTOMASSIMPUTILS_H
#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>

// Generated with ChatGPT
inline glm::mat4 toGlmMat4(const aiMatrix4x4& aiMat)
{
    glm::mat4 glmMat;

    glmMat[0][0] = aiMat.a1; glmMat[1][0] = aiMat.a2; glmMat[2][0] = aiMat.a3; glmMat[3][0] = aiMat.a4;
    glmMat[0][1] = aiMat.b1; glmMat[1][1] = aiMat.b2; glmMat[2][1] = aiMat.b3; glmMat[3][1] = aiMat.b4;
    glmMat[0][2] = aiMat.c1; glmMat[1][2] = aiMat.c2; glmMat[2][2] = aiMat.c3; glmMat[3][2] = aiMat.c4;
    glmMat[0][3] = aiMat.d1; glmMat[1][3] = aiMat.d2; glmMat[2][3] = aiMat.d3; glmMat[3][3] = aiMat.d4;

    return glmMat;
}


#endif //CUSTOMASSIMPUTILS_H
