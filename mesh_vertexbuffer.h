#ifndef MESH_VERTEXBUFFER_H
#define MESH_VERTEXBUFFER_H

#include "BoundingVolume.h"
#include <QOpenGLFunctions>

#include "vertex.h"
#include "constants.h"


class Mesh_vertexbuffer : protected QOpenGLFunctions
{
public:
    Mesh_vertexbuffer();

    int initGeometry(Vertex *verticesIn, int noOfVertecies, GLushort *indicesIn, int noOfIndecies, int lodlevelIn = 0);

    void BindBuffers(int lodin = 0); //this functions store the mVetices and mIndices numbers

    void setBoundingVolume(BoundingVolume* boundingVolumeIn);
    BoundingVolume* getBoundingVolume() const;

    unsigned int NumberofVertices(int lodIn) const;

    unsigned int NumberofIndices(int lodIn) const;
    int getlodLevels() const;

    //that we need to render them in other classes, mesh class, where we make the objects
private:
    class MeshBase *mMeshBase;
    GLuint mVertexBuffer[Orf::lodLevels]{0};
    GLuint mIndexBuffer[Orf::lodLevels]{0}; //the new way to declare a variable
    //disse blir brukt sånn som de som arver fra GameObject
    BoundingVolume* mBoundingVolume;
    unsigned int mNumberofVertices[Orf::lodLevels]{0};
    unsigned int mNumberofIndices[Orf::lodLevels]{0};
    int mLodLevel{0};
};

#endif // MESH_VERTEXBUFFER_H
