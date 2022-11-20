#include "circlesphere.h"
#include <cmath>
#include "vertex.h"
#include "vec3.h"
#include "vec2.h"

CircleSphere::CircleSphere(int resolution, float xPos, float yPos, float zPos, QString name, QString type) : GameObject(xPos, yPos, zPos, name, type)
{
    setResolution(resolution);
    initGeometry();
}

int CircleSphere::initGeometry()
{
	float *x = new float[mVertexPrCircle]{};
	float *y = new float[mVertexPrCircle]{};

    double angle{};

	for(int i = 0; i < mVertexPrCircle; i++)
    {
        angle = 2 * 3.1418f * i / mVertexPrCircle;
		x[i] = cos(angle)*mRadius;
		y[i] = sin(angle)*mRadius;
        //gl.glVertex2d(x,y);
    }

    //Format: Position, Normal, UV

	// Vertex data for circle in xy-plane
	// Change color for next circle
    Vertex *vertices = new Vertex[mVertexPrCircle*3]; //inferior stops
	mColor = Vec3(0.0f, 0.0f, 1.0f);
    // Vertex data for circle in xy-plane
	for (int i = 0; i < mVertexPrCircle; i++)
    {
		Vertex temp{};
		temp.set_xyz(x[i], y[i],  0.0f);
		temp.set_normal(mColor);
		temp.set_st(0.0f, 0.0f);
        vertices[i] = temp;
    }

    // Vertex data for circle in xz-plane
	// Change color for next circle
	mColor = Vec3(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < mVertexPrCircle; i++)
    {
		Vertex temp{};
		temp.set_xyz(x[i], 0.0f, y[i]);
		temp.set_normal(mColor);
		temp.set_st(0.0f, 0.0f);
		vertices[mVertexPrCircle + i] = temp;
    }

    // Vertex data for circle in yz-plane
	// Change color for next circle
	mColor = Vec3(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < mVertexPrCircle; i++)
    {
		Vertex temp{};
		temp.set_xyz(0.0f, y[i], x[i]);
		temp.set_normal(mColor);
		temp.set_st(0.0f, 0.0f);
		vertices[(mVertexPrCircle * 2) + i] = temp;
    }

    //empty vertecies that has to be drawn:
    //this is because we use glDrawElements(GL_LINE_STRIP ....
    //for the 3. circle to begin correct, we draw on top of first circle
    //until we get to the crossing point
	int empty = ((mVertexPrCircle / 8) - 1) * 2 + 1;

    //if mVertexPrCircle == 4, empty becomes -1
    //so we correct that.
    if (empty < 0)
        empty = 0;

    // 3 circles + 3 extra to end each circle
    // + empty drawings to start 3. circle correct
	mNoOfIndices = mVertexPrCircle*3 + 3 + empty;
	GLushort *indices = new GLushort[mNoOfIndices];

    //trying to make indecies for all 3 circles:
    int tempIndex{};

    for (int j = 0; j < 3; j++)
    {
		for (int i = 0; i< mVertexPrCircle; i++)
        {
			int temp = i + (mVertexPrCircle * (j));
            indices[tempIndex] = temp;
            tempIndex++;
        }
        //extra to end circle
		indices[tempIndex] = mVertexPrCircle * (j);

        //if j == 1 we are to start last circle
        //then we have to draw some redundant lines to
        //start last circle correct:
        if(j == 1)
        {
            for (int e = 1; e <= empty; e++)
            {
                tempIndex++;
				indices[tempIndex] = mVertexPrCircle + e;
            }
        }
        tempIndex++;
    }

    initializeOpenGLFunctions();

    // Transfer vertex data to VBO
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, (mVertexPrCircle*3*sizeof(Vertex)), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNoOfIndices*sizeof(GLushort), indices, GL_STATIC_DRAW);

    //clean up arrays we have created
    delete[] x;
    delete[] y;
    delete[] vertices;
    delete[] indices;

    return 0;
}

unsigned int CircleSphere::drawGeometry()
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

	glDrawElements(GL_LINE_STRIP, mNoOfIndices, GL_UNSIGNED_SHORT, 0);

//    //Write errors if any:
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

    return mNoOfIndices;
}

void CircleSphere::setRendermode(int mode)
{
    if (mode == 1)
		mWireFrame = true;
    else if (mode == 0)
		mWireFrame = false;
}

void CircleSphere::setMaterial(Material *materialIn)
{
	mMaterial = materialIn;
}

void CircleSphere::setResolution(int resolution)
{
    //can not have <= 0 in resolution
    if (resolution <= 0)
        resolution = 1;

    int temp{2};
    //calculate vertecies
    //1 = 4 vertecies, 2 = 8 vertecies, 3 = 16, etc.
    //has to be 4, 8, 16, 32, etc. for initGeometry to be correct.
    for (int i = 1; i <= resolution; i++)
    {
        temp = temp * 2;
    }

	mVertexPrCircle = temp;
}
