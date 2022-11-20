#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QVector3D>
#include "vec3.h"

//forward declaration:
class GameObject;

struct Origo
{
    Vec3 origoPosition;
    Vec3 origoRotation;
    Vec3 origoScale;
};

class Transform
{
public:
    Transform(GameObject* owner = nullptr);
	Vec3 getPosition();
	Vec3 getRotation();
	Vec3 getScale();

    void setPosition(float x, float y, float z);
	void setPosition(Vec3 pos) { mLocalPosition = pos; }

    void setRotation(float x, float y, float z);
	void setRotation(Vec3 rot) { mLocalRotation = rot; }

    void setScale(float x, float y, float z);
	void setScale(Vec3 scale) { mLocalScale = scale; }

    void translate(float x, float y, float z);
    void rotate(float x, float y, float z);
    //void rotate(float amount, QVector3D axis);

    Origo* getOrigo() const;
    void setOrigo(Vec3 position, Vec3 rotation, Vec3 scale);

private:
    GameObject* mGameObject;
    Origo *mOrigo;
    Vec3 mLocalPosition;
	Vec3 mLocalRotation;
	Vec3 mLocalScale;
};

#endif // TRANSFORM_H
