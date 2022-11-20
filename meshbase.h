#ifndef MESHBASE_H
#define MESHBASE_H

#include <QOpenGLFunctions>
#include "boundingvolume.h"
#include "constants.h"

class MeshBase : protected QOpenGLFunctions
{
public:
    MeshBase();

    int initGeometry(class Vertex *verticesIn, unsigned int noOfVertecies, GLushort *indicesIn = 0, unsigned int noOfIndecies = 0, int lodlevelIn = 0 );

    void useMesh(int lodin);

    void setBoundingVolume(BoundingVolume *boundingVolumeIn);
    BoundingVolume *getBoundingVolume() const;

    unsigned int NumberofVertices(int lodIn) const;

    unsigned int NumberofIndices(int lodIn) const;
    int getlodLevels() const;

private:
    GLuint mVertexBuffer[3];
    GLuint mIndexBuffer[3];

    BoundingVolume *mBoundingVolume;
        int mLodLevel{0};
        unsigned int mNumberofVertices[Orf::lodLevels]{0};
        unsigned int mNumberofIndices[Orf::lodLevels]{0};

};

#endif // MESHBASE_H
