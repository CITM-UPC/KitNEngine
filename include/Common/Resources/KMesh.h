//
// Created by Roger on 25/12/2024.
//

#ifndef MESH_H
#define MESH_H
#include <vector>

class KMesh {
public:
    explicit KMesh(const char* filepath);
    bool InUse();

    
    bool onGPU = false;
    unsigned int numVertices = 0;
    unsigned int numFaces = 0;
    unsigned int bufferID = 0;
};



#endif //MESH_H
