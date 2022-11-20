#include "skybox.h"
#include "vertex.h"
#include "vec3.h"
#include "vec2.h"

SkyBox::SkyBox(float xPos, float yPos, float zPos, QString name, QString type) : GameObject(xPos, yPos, zPos, name, type)
{
    initGeometry();
}

unsigned int SkyBox::drawGeometry()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	mMaterial->useMaterial();

    //refresh modelMatrix:
    getMatrix();

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

    //Want to draw the inside of the cube
    //Could have changed the indices-order.
    //That would be more correct
    glFrontFace(GL_CW);

    // Draw cube geometry using indices from VBO 1
	if (!mWireFrame)
        glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
    else
        glDrawElements(GL_LINES, 34, GL_UNSIGNED_SHORT, 0);

    //Turn of "inside draw" hack
    glFrontFace(GL_CCW);

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

    return 34;
}

int SkyBox::initGeometry()
{
    //For now hard coded as a cube:

    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.

    //qDebug() << "Init Geometry - Cube";
    //Format: Position, Normal, UV
	Vertex vertices[] = {
        // Vertex data for face 0 - front
		{Vec3(-1.0f, -1.0f,  1.0f), Vec3(0.0f, 0.0f,  -1.0f), Vec2(0.0f, 0.0f)},// v0
		{Vec3( 1.0f, -1.0f,  1.0f), Vec3(0.0f, -1.0f,  -1.0f), Vec2(0.33f, 0.0f)}, // v1
		{Vec3(-1.0f,  1.0f,  1.0f), Vec3(0.0f, 0.0f,  -1.0f), Vec2(0.0f, 0.5f)},// v2
		{Vec3( 1.0f,  1.0f,  1.0f), Vec3(0.0f, -1.0f,  -1.0f), Vec2(0.33f, 0.5f)},// v3

        // Vertex data for face 1 - right
		{Vec3( 1.0f, -1.0f,  1.0f), Vec3(-1.0f, -1.0f,  -1.0f), Vec2( 0.0f, 0.5f)},// v4
		{Vec3( 1.0f, -1.0f, -1.0f), Vec3(-1.0f, 0.0f,  0.0f), Vec2(0.33f, 0.5f)},// v5
		{Vec3( 1.0f,  1.0f,  1.0f), Vec3(-1.0f, 0.0f,  -1.0f), Vec2(0.0f, 1.0f)}, // v6
		{Vec3( 1.0f,  1.0f, -1.0f), Vec3(-1.0f, 0.0f,  0.0f), Vec2(0.33f, 1.0f)},  // v7

        // Vertex data for face 2 - back
		{Vec3( 1.0f, -1.0f, -1.0f), Vec3(0.0f, 0.0f,  1.0f), Vec2(0.66f, 0.5f)},// v8
		{Vec3(-1.0f, -1.0f, -1.0f), Vec3(0.0f, 0.0f,  1.0f), Vec2(1.0f, 0.5f)},// v9
		{Vec3( 1.0f,  1.0f, -1.0f), Vec3(0.0f, -1.0f,  1.0f), Vec2(0.66f, 1.0f)}, // v10
		{Vec3(-1.0f,  1.0f, -1.0f), Vec3(0.0f, 0.0f,  1.0f), Vec2(1.0f, 1.0f)},// v11

        // Vertex data for face 3 - left
		{Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 0.0f,  0.0f), Vec2(0.66f, 0.0f)},// v12
		{Vec3(-1.0f, -1.0f,  1.0f), Vec3(1.0f, 0.0f,  -1.0f),Vec2(1.0f, 0.0f)}, // v13
		{Vec3(-1.0f,  1.0f, -1.0f), Vec3(1.0f, 0.0f,  0.0f), Vec2(0.66f, 0.5f)},// v14
		{Vec3(-1.0f,  1.0f,  1.0f), Vec3(1.0f, 0.0f,  0.0f), Vec2(1.0f, 0.5f)}, // v15

        // Vertex data for face 4 - bottom
		{Vec3(-1.0f, -1.0f, -1.0f), Vec3(0.0f, 1.0f,  0.0f), Vec2(0.33f, 0.0f)},// v16
		{Vec3( 1.0f, -1.0f, -1.0f), Vec3(0.0f, 1.0f,  0.0f), Vec2(0.66f, 0.0f)},// v17
		{Vec3(-1.0f, -1.0f,  1.0f), Vec3(0.0f, 1.0f,  0.0f), Vec2(0.33f, 0.5f)}, // v18
		{Vec3( 1.0f, -1.0f,  1.0f), Vec3(0.0f, 1.0f,  0.0f),Vec2(0.66f, 0.5f)}, // v19

        // Vertex data for face 5 - top
		{Vec3(-1.0f,  1.0f,  1.0f), Vec3(0.0f, -1.0f,  0.0f), Vec2(0.33f, 0.5f)}, // v20
		{Vec3( 1.0f,  1.0f,  1.0f), Vec3(-1.0f, -1.0f,  0.0f), Vec2(0.66f, 0.5f)},// v21
		{Vec3(-1.0f,  1.0f, -1.0f), Vec3(0.0f, -1.0f,  0.0f), Vec2(0.33f, 1.0f)}, // v22
		{Vec3( 1.0f,  1.0f, -1.0f), Vec3(0.0f, -1.0f,  0.0f), Vec2(0.66f, 1.0f)} // v23
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
        0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
        4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
        8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

    initializeOpenGLFunctions();

    // Transfer vertex data to VBO 0
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 34*sizeof(GLushort), indices, GL_STATIC_DRAW);

    return 0;
}

void SkyBox::setRendermode(int mode)
{
    if (mode == 1)
		mWireFrame = true;
    else if (mode == 0)
		mWireFrame = false;
}

void SkyBox::setMaterial(Material *materialIn)
{
	mMaterial = materialIn;
}
