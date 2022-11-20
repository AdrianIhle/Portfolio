//author lower methodes, Kaya Karlsen

#include "Plane.h"
#include "vertex.h"
#include <QImage>
#include "cmath"

Plane::Plane(float xPos, float yPos, float zPos, int mSize, float mSpacing, QString name, QString type) : GameObject(xPos, yPos, zPos, name, type)
{
    // Initializes map geometry and transfers it to VBOs
    mapSize = mSize;
    mapSpacing = mSpacing;
    initGeometry();
}


int Plane::initGeometry()
{
    //For height maps:
       //QImage *height = new QImage("Orf::assetFilePath + height.jpg");
       //    for (int i = 0; i< mapSize*2; i++)
       //    {
       //        qDebug() << i << " " << qBlue(height->pixel(i, 0));
       //    }

       //Make the array for all the vertecies:

       vertices = new Vertex[mapSize*mapSize];


       //Double for-loop to make the vertecies by row and coloumn
       for(int row = 0; row < mapSize; row++)
       {
           for(int col = 0; col < mapSize; col++)
           {
               vertices[col + (row * mapSize)] = {

                       //This is what I use when reading height from heightmap:
                       //              x,                 y (-128 for height +-128),                            z
                       //Vec3(col*mapFactor, (float(qBlue(height->pixel(col, row)))-128.0f)/heightFactor,  row*mapFactor),


                       //The height (y-value) here is just an arbertrary formula to get some variation:
                       Vec3(col*mapSpacing, cos(row+col),  row*mapSpacing),
                       Vec3(float(col)/mapSize, float(row)/mapSize,  float(col)/8),  //normal - not real normal calculation...
                       Vec2(0.0f, 0.0f)} ;        //texture coordinates
           }
       }

       // Calculate how may indicies the map needs:
       // each row = mapsize * 2
       // number of rows = mapSize -1
       // between rows we need 2 extra indicies for empty triangles
       indexCount = mapSize * 2 * (mapSize-1) + (mapSize-2) * 2;

       GLushort *indices = new GLushort[indexCount];
       int indexIndicies = 0;

       for(int row = 0; row < mapSize; row++)
       {
           //make coloumns
           for(int col = 0; col < (mapSize); col++)
           {
               indices[indexIndicies] = row * mapSize + col;
               indexIndicies++;
               indices[indexIndicies] = row * mapSize + col + mapSize;
               //         0, 4, 1, 5, 2, 6, 3, 7,       // 0, 0+size, 1, 1+size, etc.
               //         7, 4,   //degenerated, empty    // last of previous, first of comming
               //         4, 8, 5, 9, 6, 10, 7, 11,       // row*size+col, size*2+1, row*size+col, size*2+2, etc.
               //         11, 8,  //degenerated, empty    // last of previous, first of comming
               //         8, 12, 9, 13, 10, 14, 11, 15    // size*2, size*3+1, size*2+1, size*3+2, etc.
               indexIndicies++;
           }
           //make empty triangles before next row
           if (row < (mapSize-2))
           {
               indices[indexIndicies] = indices[indexIndicies-1];
               indexIndicies++;
               indices[indexIndicies] = (row+1) * mapSize;
               indexIndicies++;
           }
           if (indexIndicies == indexCount )
               break;

       }

       // Transfer vertex data to VBO 0
       refreshVertices();

       // Transfer index data to VBO 1
       glGenBuffers(1, &mIndexBuffer);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(GLushort), indices, GL_STATIC_DRAW);

      // delete[] vertices;

       return 0;
   }

unsigned int Plane::drawGeometry()
{
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
        mMaterial->useMaterial();

        //refresh modelMatrix:
        getMatrix();

        mMVPMatrix = mProjectionMatrix*mViewMatrix*mModelMatrix;
        mMaterial->setMVPMatrix(mMVPMatrix);

        // Offset for position
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

        // Draw cube geometry using indices from VBO 1
        if (!mWireFrame)
            glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_SHORT, 0);
        else
            glDrawElements(GL_LINE_STRIP, indexCount, GL_UNSIGNED_SHORT, 0);

        //Write errors if any:
    //    GLenum err = GL_NO_ERROR;
    //    while((err = glGetError()) != GL_NO_ERROR)
    //    {
    //        qDebug() << "glGetError returns " << err;
    //    }

        //Unbind buffers:
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //Unbind shader
        glUseProgram(0);

        return indexCount;
}

void Plane::setRendermode(int mode)
{
    if (mode == 1)
        mWireFrame = true;
    else if (mode == 0)
        mWireFrame = false;
}

void Plane::setMaterial(Material *materialIn)
{
    mMaterial = materialIn;
}

void Plane::refreshVertices()
{
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mapSize*mapSize*sizeof(Vertex), vertices, GL_STATIC_DRAW);
}
