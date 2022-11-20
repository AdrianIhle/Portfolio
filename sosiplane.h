#ifndef SOSIPLANE_H
#define SOSIPLANE_H
#include "gameobject.h"
#include "vertex.h"
#include "material.h"

class SOSIplane : public GameObject
{
public:
    SOSIplane(QString path = "", float scale = 1.0f, float heightDivide = 1.0f, float xPos = 0.0f, float yPos = 0.0f, float zPos = 0.0f, QString nameIn = "SosiMap");
    unsigned int drawGeometry();
    void setMaterial(Material* materialIn);
    void setRendermode(int mode);

private:
    int initGeometry();
    int initGeometry(std::string filePath);
    float mScale;
    Vertex* mVertices;
    GLushort* mIndices;
    float mHeightScale;
    int numOfVertices{0};
    int numOfIndices{0};
    GLuint mVertexBuffer;
    GLuint mIndexBuffer;
    Material* mMaterial;
    QMatrix4x4 mMVPMatrix;
};

#endif // SOSIPLANE_H
