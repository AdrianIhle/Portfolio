#ifndef AXESGIZMO_H
#define AXESGIZMO_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include "transform.h"
#include "gameobject.h"
#include "material.h"

class AxesGizmo : public GameObject
{
public:
    AxesGizmo(float xPos = 0.0f, float yPos = 0.0f, float zPos = 0.0f);

    unsigned int drawGeometry();
    void setMaterial(Material *materialIn);
	void setRendermode(int mode);

private:
    int initGeometry();

	GLuint mVertexBuffer;
	GLuint mIndexBuffer;

	Material *mMaterial;

	QMatrix4x4 mMVPMatrix;
};

#endif // AXESGIZMO_H
