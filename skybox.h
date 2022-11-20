#ifndef SKYBOX_H
#define SKYBOX_H

#include "gameobject.h"
#include "material.h"

class SkyBox : public GameObject
{
public:
    SkyBox(float xPos = 0.0, float yPos = 0.0, float zPos = 0.0, QString name = "SkyBox", QString type = "Skybox");

    unsigned int drawGeometry();
    int initGeometry();
    void setRendermode(int mode);

    void setMaterial(Material *materialIn);

private:
	GLuint mVertexBuffer;
	GLuint mIndexBuffer;

	Material *mMaterial;

	QMatrix4x4 mMVPMatrix;
};

#endif // SKYBOX_H
