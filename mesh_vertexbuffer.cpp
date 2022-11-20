#include "Mesh_vertexbuffer.h"

Mesh_vertexbuffer::Mesh_vertexbuffer()
{

}

int Mesh_vertexbuffer::initGeometry(Vertex *verticesIn, int noOfVertecies, GLushort *indicesIn, int noOfIndecies, int lodlevelIn)
{
    if(lodlevelIn < 0 || lodlevelIn > Orf::lodLevels - 1)  //if lodLevel is out of range - return error
         return 1;

    mNumberofIndices[lodlevelIn] = noOfIndecies;

    initializeOpenGLFunctions();
    // Transfer vertex data to VBO 0
    glGenBuffers(1, &mVertexBuffer[lodlevelIn]);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[lodlevelIn]);
    glBufferData(GL_ARRAY_BUFFER, noOfVertecies*sizeof(Vertex), verticesIn, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    if(indicesIn != 0)
    {
        glGenBuffers(1, &mIndexBuffer[lodlevelIn]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer[lodlevelIn]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, noOfIndecies*sizeof(GLushort), indicesIn, GL_STATIC_DRAW);
    }
    //can be used to send errors. 0 = no errors
    mNumberofVertices[lodlevelIn] = noOfVertecies;
    mLodLevel++; //så neste gang den kjører så vet vi at den er på lod 2, Synkronisert med mesh_file
    return 0;

}

void Mesh_vertexbuffer::BindBuffers(int lodin)
{
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[lodin]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer[lodin]);
}

void Mesh_vertexbuffer::setBoundingVolume(BoundingVolume *boundingVolumeIn)
{
    mBoundingVolume = boundingVolumeIn;
}

BoundingVolume *Mesh_vertexbuffer::getBoundingVolume() const
{
    return mBoundingVolume;
}

unsigned int Mesh_vertexbuffer::NumberofVertices(int lodIn) const
{
    return mNumberofVertices[lodIn];
}

unsigned int Mesh_vertexbuffer::NumberofIndices(int lodIn) const
{
    return mNumberofIndices[lodIn];
}

int Mesh_vertexbuffer::getlodLevels() const
{
    return mLodLevel;
}
