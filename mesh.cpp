#include "mesh.h"
#include "vec3.h"
#include "vec2.h"
#include "boundingvolume.h"
#include "meshbase.h"

Mesh::Mesh(float xPos, float yPos, float zPos, QString name, QString type) : GameObject(xPos, yPos, zPos, name, type)
{
}

unsigned int Mesh::drawGeometry()
{
    mMeshBase->useMesh(lodLevel);
    mIndicesSize = mMeshBase->NumberofIndices(lodLevel);
    mVerticeSize = mMeshBase->NumberofVertices(lodLevel);
    mMaterial->useMaterial();

    //refresh modelMatrix:
    getMatrix();

    if(frustrumDraw)
    {
        mMVPMatrix = mProjectionMatrix*mViewMatrix*mModelMatrix.inverted();
    }
    else
        mMVPMatrix = mProjectionMatrix*mViewMatrix*mModelMatrix;
    mMaterial->setMVPMatrix(mMVPMatrix);

    // Offset for position
    // Positions are the first data, therefor offset is 0
    int offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getPositionAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    // Offset for vertex coordinates
    // before normals
    offset += sizeof(Vec3);

    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getNormalAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    // Offset for normal coordinates
    // before UVs
    offset += sizeof(Vec3);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getTextureAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    if (!mWireFrame && mIndicesSize != 0)
        glDrawElements(GL_TRIANGLES, mIndicesSize, GL_UNSIGNED_SHORT, 0);
    else if(mIndicesSize != 0)
        glDrawElements(GL_LINE_STRIP, mIndicesSize, GL_UNSIGNED_SHORT, 0);
    //Write errors if any:
    //    GLenum err = GL_NO_ERROR;
    //    while((err = glGetError()) != GL_NO_ERROR)
    //    {
    //        qDebug() << "glGetError returns " << err;
    //    }

    if(mType == "Mankey")
    {
        qDebug() << getType();
    }

    //Unbind buffers:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//        if (mSelectedInEditor)
//            mBoundingVolume->drawGeometry(mMaterial);

    //Unbind shader
    glUseProgram(0);

    return mMeshBase->NumberofVertices(lodLevel);
}

//To be compatible with base class:
//This is not optimal, so I should reconsider this system
int Mesh::initGeometry(MeshBase *m)
{

    mMeshBase = m;
    mBoundingVolume = mMeshBase->getBoundingVolume();

    return 0;
}



void Mesh::setRendermode(int mode)
{
    if (mode == 1)
        mWireFrame = true;
    else if (mode == 0)
        mWireFrame = false;
}

void Mesh::setMaterial(Material *materialIn)
{
    mMaterial = materialIn;
}



void Mesh::setViewMatrix(QMatrix4x4 viewMatrix)
{
    mViewMatrix = viewMatrix;
}

void Mesh::setprojectionMatrix(QMatrix4x4 projectionMatrix)
{
    mProjectionMatrix = projectionMatrix;
}

MeshBase *Mesh::mMeshBuffer()
{
    return mMeshBase;
}

void Mesh::setLodLevel(int lodLevelIn)
{
    //  if (mMeshBase->getLodLevels() > lodLevel)
    lodLevel = lodLevelIn;
}
