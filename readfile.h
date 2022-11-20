#ifndef READFILE_H
#define READFILE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "mesh.h"
#include "vertex.h"
#include "vec3.h"

using std::cout;
using std::endl;

class ReadFile
{
public:
    ReadFile(std::string filePath);
    Mesh* mNewMesh;
    void ReadFileToMesh();
private:
    Vertex* nVerticies;
    GLushort* nIndicies;
    Material* nMaterials;
    std::string mFileName;
    std::string mOneLine;
    std::string mOneWord;

protected:

};

#endif // READFILE_H
