#include "axesgizmo.h"
#include "vertex.h"

AxesGizmo::AxesGizmo(float xPos, float yPos, float zPos)
{
    mTransform = new Transform(this);
	mTransform->setPosition(xPos, yPos, zPos);

    initGeometry();
    getMatrix();
}

int AxesGizmo::initGeometry()
{
    //Format: Position, UV, Normal
	Vertex vertices[6];

        // Vertex data for X-axix

		vertices[0].set_xyz(-500.0f*mTransform->getScale().x, 0.0f,  0.0f);
		vertices[1].set_xyz(500.0f*mTransform->getScale().x, 0.0f,  0.0f);
		vertices[2].set_xyz(0.0f, -500.0f*mTransform->getScale().y, 0.0f);
		vertices[3].set_xyz(0.0f,  500.0f*mTransform->getScale().x, 0.0f);
		vertices[4].set_xyz(0.0f, 0.0f, -500.0f*mTransform->getScale().z);
		vertices[5].set_xyz(0.0f, 0.0f,  500.0f*mTransform->getScale().x);
		vertices[0].set_rgb(1.0, 0.0, 0.0);
		vertices[1].set_rgb(1.0, 0.0, 0.0);
		vertices[2].set_rgb(0.0, 1.0, 0.0);
		vertices[3].set_rgb(0.0, 1.0, 0.0);
		vertices[4].set_rgb(0.0, 0.0, 1.0);
		vertices[5].set_rgb(0.0, 0.0, 1.0);

	GLushort indices[] = {
		0, 1, 2, 3, 4, 5
	};

    initializeOpenGLFunctions();

    // Transfer vertex data to VBO 0
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Transfer index data to VBO 1
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLushort), indices, GL_STATIC_DRAW);

    return 0;
}

unsigned int AxesGizmo::drawGeometry()
{
    // Tell OpenGL which VBOs to use
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	mMaterial->useMaterial();

    //refresh modelMatrix:
    //should not be neccecary since axes will not move:
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
	offset += sizeof(QVector3D);

	glVertexAttribPointer(static_cast<GLuint>(mMaterial->getNormalAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    // Offset for normal coordinates
    // before UVs
	offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
	glVertexAttribPointer(static_cast<GLuint>(mMaterial->getTextureAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    glDrawElements(GL_LINES, 6, GL_UNSIGNED_SHORT, 0);

    // Write errors if any:
//    GLenum err = GL_NO_ERROR;
//    while((err = glGetError()) != GL_NO_ERROR)
//    {
//        qDebug() << "glGetError returns " << err;
//    }

    return 6;
}

void AxesGizmo::setMaterial(Material *materialIn)
{
	mMaterial = materialIn;
}

void AxesGizmo::setRendermode(int mode)
{
	if (mode == 1)
		mWireFrame = true;
	else if (mode == 0)
		mWireFrame = false;
}
