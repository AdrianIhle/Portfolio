#ifndef MESH_H
#define MESH_H

#include <QString>
#include "Mesh_vertexbuffer.h"
#include "gameobject.h"
#include "BoundingVolume.h"
#include "meshbase.h"
#include "vertex.h"

class Mesh : public GameObject
{
public:
    Mesh(float xPos = 0.0, float yPos = 0.0, float zPos = 0.0, QString name = "Generic Mesh", QString type = "Mesh") ;

    unsigned int drawGeometry();
    int initGeometry(MeshBase *m);
    void setRendermode(int mode);
    void setViewMatrix(QMatrix4x4 viewMatrix);
    void setprojectionMatrix(QMatrix4x4 projectionMatrix);
    void setLodLevel(int lodLevelIn);
    MeshBase *mMeshBuffer();

    bool frustrumDraw;

    void setMaterial(class Material *materialIn);

private:


    class MeshBase *mMeshBase;
    class Material *mMaterial;
    class QMatrix4x4 mMVPMatrix;

    unsigned int mIndicesSize{0};
    unsigned int mVerticeSize{0};

    BoundingVolume * mBoundingVolume;
        int lodLevel{0};
};

#endif // MESH_H
