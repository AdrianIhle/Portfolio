#include "BoundingVolume.h"
#include "gameobject.h"

BoundingVolume::BoundingVolume()
{

}

int BoundingVolume::initGeometry(Vertex *vertices, int NumofVertices)
{
    //have to find vertex max x, min x, max y, min y
    //vertices that gets sends inn is mVertices that is used by each object
    //that has control over the different vertices

    //The x and y is the same for now
    float xMax = vertices[0].get_x();
    float xMin = vertices[0].get_x();

    float yMax = vertices[0].get_y();
    float yMin = vertices[0].get_y();

    float zMax = vertices[0].get_z();
    float zMin = vertices[0].get_z();

    //here we uses the number of vertices to loop through them all
    for(int i = 0; i < NumofVertices; i++)
    {
        //it checks if current vertice x or y is less or more than
        //the smallest/biggets x. i has control over how many times we need to loop through
        //first vertice is gonna say it is the least cause right now
        // xMin is just empty. When it loops next time, it has something to
        //compare it to
        if(vertices[i].get_x() > xMin)
            xMin = vertices[i].get_x();
        if(vertices[i].get_x() < xMax)
            xMax = vertices[i].get_x();

        if(vertices[i].get_y() > yMin)
            yMin = vertices[i].get_y();
        if(vertices[i].get_y() < yMax)
            yMax = vertices[i].get_y();

        if(vertices[i].get_z() > yMin)
            zMin = vertices[i].get_z();
        if(vertices[i].get_z() < zMax)
            zMax = vertices[i].get_z();
    }

    //color to the outline
    Vec3 color(1.0f, 0.5f, 0.2f);
    Vec2 texture(0.0f, 0.0f);

    Vertex vertice[] = {
        {Vec3(xMin, yMax, zMax), color, texture},
        {Vec3(xMin, yMin, zMax), color, texture},
        {Vec3(xMax, yMax, zMax), color, texture},
        {Vec3(xMax, yMin, zMax), color, texture},

        {Vec3(xMin, yMax, zMin), color, texture},
        {Vec3(xMin, yMin, zMin), color, texture},
        {Vec3(xMax, yMax, zMin), color, texture},
        {Vec3(xMax, yMin, zMin), color, texture}
    };

    GLushort indices[] = {
        0, 1, 2, 3, 0,
        4, 5, 6, 7, 4,
        5, 1, 2, 6, 7, 3
    };

    initializeOpenGLFunctions();

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(Vertex), vertice, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 16*sizeof(GLushort), indices, GL_STATIC_DRAW);

    return 0;
}

unsigned int BoundingVolume::drawGeometry(Material *mMaterial)
{

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

    //mMVPMatrix = mProjectionMatrix*mViewMatrix*mModelMatrix;
    //mMaterial->setMVPMatrix(mMVPMatrix);

    // Offset for position
    // Positions are the first data, therefor offset is 0
    int offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getPositionAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    // Offset for vertex coordinates
    // before normals
    offset += sizeof(Vec3);//instead of Vec3 QVector3D is what Ole uses. Why??

    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getNormalAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    // Offset for normal coordinates
    // before UVs
    offset += sizeof(Vec3);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getTextureAttribute()), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    //en glLineWidth specify the width rastiereized line.
    //to rastireized is pixelated that can be rendered.
    //This is used to make lines around the cube bigger, so that it's easier to
    //see that the object has been selected
    //må ha glLineWidth først for å gjøre den større for vi drawer
    //må gjøre LineWidth normal igjen etterpå så de andre objektene i scene ikke blir påvirket
    glLineWidth(5.0f);
    glDrawElements(GL_LINE_LOOP, 16, GL_UNSIGNED_SHORT, 0);
    glLineWidth(GLfloat(1.0f));

    //Unbind buffers:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Unbind shader
    glUseProgram(0);

    return 16;
}
