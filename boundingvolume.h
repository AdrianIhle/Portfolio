#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <QOpenGLFunctions>
#include "vertex.h"
#include "material.h"

class BoundingVolume : protected QOpenGLFunctions
{
public:
    BoundingVolume();

    int initGeometry(Vertex* vertices, int NumofVertices);
    unsigned int drawGeometry(Material* mMaterial);


private:
    GLuint mVertexBuffer;
    GLuint mIndexBuffer;

    //BoundingVolume* mBoundingVolume;

};

#endif // BOUNDINGVOLUME_H
