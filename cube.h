#ifndef CUBE_H
#define CUBE_H

#include "gameobject.h"
#include "vertex.h"
#include "material.h"

class Cube : public GameObject
{
public:
    Cube(float xPos = 0.0, float yPos = 0.0, float zPos = 0.0, QString name = "Cube", QString type = "Cube");

    unsigned int drawGeometry();
    int initGeometry();
    void setRendermode(int mode);

    void setMaterial(Material *materialIn);

private:
    GLuint mVertexBuffer;
    GLuint mIndexBuffer;

    Material *mMaterial;

    QMatrix4x4 mMVPMatrix;

};

#endif // CUBE_H
