//
// Created by Roger on 09/10/2024.
//

#ifndef MODEL3D_H
#define MODEL3D_H

struct MyMesh
{
    public:
    explicit MyMesh(const aiMesh* mesh)
    {
        vertices.reserve(mesh->mNumVertices*3);
        for (unsigned int i = 0; i < mesh->mNumVertices;i++)
        {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);
        }
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace& face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
    }

    std::vector<float> vertices;
    std::vector<glm::uint> indices;
};

#endif //MODEL3D_H
