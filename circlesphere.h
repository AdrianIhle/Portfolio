#ifndef CIRCLESPHERE_H
#define CIRCLESPHERE_H

#include "gameobject.h"
#include "vertex.h"
#include "material.h"
#include "vec3.h"

/**
 * @brief The CircleSphere class
 */
class CircleSphere : public GameObject
{
public:
    CircleSphere(int resolution = 1, float xPos = 0.0, float yPos = 0.0, float zPos = 0.0, QString name = "CircleSphere", QString type = "Criclesphere");

    unsigned int drawGeometry();
    int initGeometry();
    void setRendermode(int mode);

    void setMaterial(Material *materialIn);

private:
    void setResolution(int resolution);
	GLuint mVertexBuffer;
	GLuint mIndexBuffer;

	Material *mMaterial;

	QMatrix4x4 mMVPMatrix;

	int mVertexPrCircle;
	float mRadius = 3.0f;
	int mNoOfIndices;
	Vec3 mColor{1.0f, 1.0f, 1.0f};
};

#endif // CIRCLESPHERE_H
