//
// Created by Roger on 25/12/2024.
//

#ifndef MESHIMPORTER_H
#define MESHIMPORTER_H
#include "Common/Resources/KMesh.h"


struct aiMesh;

class MeshImporter {
public:
    void Import(const char* filename);
    void Save(KMesh mesh);
    void Load();

private:

    void ImportMesh(const aiMesh* mesh);
};



#endif //MESHIMPORTER_H
