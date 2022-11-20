#include "ball.h"

// OktaederBall
// Approksimasjon av kule med rekursiv oktaeder subdivisjon
#include <iostream>
#include <sstream>
#include "vertex.h"


ball::ball(float xPs, float yPs, float zPs, int n, QString name) : m_rekursjoner(n), m_indeks(0), GameObject (xPs, yPs,zPs,name)
{
    m_antallVertices = 3 * 8 * pow(4, m_rekursjoner);
    mVertices = new Vertex[m_antallVertices];
    oktaederUnitBall();
    initGeometry();
}
ball::~ball()
{
    delete [] mVertices;
}

//!
void ball::lagTriangel(const Vec3& v1, const Vec3& v2, const Vec3& v3)
{
    mVertices[m_indeks].set_xyz(v1.x, v1.y, v1.z);
    mVertices[m_indeks].set_normal(v1.x, v1.y, v1.z);
    mVertices[m_indeks++].set_st(0.0f, 0.0f);
    mVertices[m_indeks].set_xyz(v2.x, v2.y, v2.z);
    mVertices[m_indeks].set_normal(v2.x, v2.y, v2.z);
    mVertices[m_indeks++].set_st(1.0f, 0.0f);
    mVertices[m_indeks].set_xyz(v3.x, v3.y, v3.z);
    mVertices[m_indeks].set_normal(v3.x, v3.y, v3.z);
    mVertices[m_indeks++].set_st(0.5f, 1.0f);
}


void ball::subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n)
{
    if (n>0) {
        Vec3 v1 = a+b; v1.normalize();
        Vec3 v2 = a+c; v2.normalize();
        Vec3 v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    } else {
        lagTriangel(a, b, c);
    }
}


void ball::oktaederUnitBall()
{
    Vec3 v0(x+0, y+0, z+1);
    Vec3 v1(x+1, y+0, z+0);
    Vec3 v2(x+0, y+1, z+0);
    Vec3 v3(x+-1, y+0, z+0);
    Vec3 v4(x+0, y+-1, z+0);
    Vec3 v5(x+0, y+0, z+-1);

    subDivide(v0, v1, v2, m_rekursjoner);
    subDivide(v0, v2, v3, m_rekursjoner);
    subDivide(v0, v3, v4, m_rekursjoner);
    subDivide(v0, v4, v1, m_rekursjoner);
    subDivide(v5, v2, v1, m_rekursjoner);
    subDivide(v5, v3, v2, m_rekursjoner);
    subDivide(v5, v4, v3, m_rekursjoner);
    subDivide(v5, v1, v4, m_rekursjoner);
}

//!
//! \brief OktaederBall::initVertexBufferObjects() calls glGenBuffers(), glBindBuffer() and glBufferdata()
//! for using later use of glDrawArrays()
//!
void ball::initVertexBufferObjects()
{
    initializeOpenGLFunctions();



    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_antallVertices*sizeof(Vertex),mVertices, GL_STATIC_DRAW);
}

//!
//! \brief OktaederBall::draw() draws a ball using glDrawArrays()
//! \param positionAttribute    vertex shader variable for position
//! \param normalAttribute      vertex shader variable for normal or color
//! \param textureAttribute     vertex shader variable for texture coordinates (optional)
//!
//! draw()
//! - glBindBuffer()
//! - glVertexAttribPointer()
//! - glBindTexture()
//! - glVertexAttribPointer()
//! - glDrawArrays() with GL_TRIANGLES
//!

int ball::initGeometry()
{
    //For now hard coded as a cube:

    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.

    //qDebug() << "Init Geometry - Cube";
    //Format: Position, Normal, UV

    initVertexBufferObjects();

    return 0;

}

void ball::drawGeometry()
{
//    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    mMaterial->useMaterial();

//    //refresh modelMatrix:
    getMatrix();

    mMVPMatrix = mProjectionMatrix*mViewMatrix*mModelMatrix;
    mMaterial->setMVPMatrix(mMVPMatrix);

//    // Offset for position
//    // Positions are the first data, therefor offset is 0
//    int offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
//    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getPositionAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

//    // Offset for vertex coordinates
//    // before normals
//    offset += sizeof(Vec3);

//    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getNormalAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

//    // Offset for normal coordinates
//    // before UVs
//    offset += sizeof(Vec3);

//    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
//    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getTextureAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

//    // Draw cube geometry using indices from VBO 1
//    if (!mWireFrame)
//        glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
//    else
//        glDrawElements(GL_LINES, 34, GL_UNSIGNED_SHORT, 0);

//    //Write errors if any:
//    GLenum err = GL_NO_ERROR;
//    while((err = glGetError()) != GL_NO_ERROR)
//    {
//        qDebug() << "glGetError returns " << err;
//    }

    //Unbind buffers:
/*    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

    //Unbind shader
//    glUseProgram(0);

    int offset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer(mMaterial->getPositionAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*> (offset)); /// stride 3*sizeof(GL_FLOAT) går også bra!?
    offset = 3*sizeof(GLfloat);

    glVertexAttribPointer(mMaterial->getNormalAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*> (offset)); /// stride 3*sizeof(GL_FLOAT) går også bra!?

    // For senere teksturering

    glDrawArrays(GL_TRIANGLES, 0, m_antallVertices);
        glUseProgram(0);
//    glDrawArrays(GL_TRIANGLES, 0, m_antallVertices);

        //    if (!mWireFrame)
        //        glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
        //    else
        //        glDrawElements(GL_LINES, 34, GL_UNSIGNED_SHORT, 0);
}


void ball::setRendermode(int mode)
{
    if (mode == 1)
        mWireFrame = true;
    else if (mode == 0)
        mWireFrame = false;
}

void ball::setMaterial(Material *materialIn)
{
    mMaterial = materialIn;
}
