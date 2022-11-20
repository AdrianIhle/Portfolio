#include "transform.h"
#include "gameobject.h"

Transform::Transform(GameObject* owner)
{
	mLocalPosition = Vec3(0, 0, 0);
	mLocalRotation = Vec3(0, 0, 0);
	mLocalScale = Vec3(1, 1, 1);

    mGameObject = owner;

    if(owner->getParent() == nullptr)
        setOrigo(Vec3(0,0,0), Vec3(0,0,0), Vec3 (0,0,0));
    else
            setOrigo(owner->getParent()->mTransform->getPosition()
                     ,owner->getParent()->mTransform->getRotation()
                     ,owner->getParent()->mTransform->getScale());
}

Vec3 Transform::getPosition()
{
    if (mGameObject->getParent())
    {
		Vec3 parentPos = mGameObject->getParent()->mTransform->getPosition();
        return mLocalPosition + parentPos;
    }

    return mLocalPosition;
}

Vec3 Transform::getRotation()
{
    if (mGameObject->getParent())
    {
		Vec3 parentRot = mGameObject->getParent()->mTransform->getRotation();
        return mLocalRotation + parentRot;
    }

    return mLocalRotation;
}

Vec3 Transform::getScale()
{
    if (mGameObject->getParent() != nullptr)
    {
		Vec3 parentScale = mGameObject->getParent()->mTransform->getScale();
        return mLocalScale + parentScale;
    }

    return mLocalScale;
}

void Transform::setPosition(float x, float y, float z)
{
    mLocalPosition = mOrigo->origoPosition + Vec3(x, y, z);
}

void Transform::setRotation(float x, float y, float z)
{
    mLocalRotation = mOrigo->origoRotation + Vec3(x, y, z);
}

void Transform::setScale(float x, float y, float z)
{
    mLocalScale = mOrigo->origoScale + Vec3(x, y, z);
}

void Transform::translate(float x, float y, float z)
{
	mLocalPosition = mLocalPosition + Vec3(x, y, z);
}

void Transform::rotate(float x, float y, float z)
{
	mLocalRotation = mLocalRotation + Vec3(x, y, z);
}

Origo *Transform::getOrigo() const
{
    return mOrigo;
}

void Transform::setOrigo(Vec3 position, Vec3 rotation, Vec3 scale)
{
    mOrigo = new Origo();
    mOrigo->origoPosition = position;
    mOrigo->origoRotation  = rotation;
    mOrigo->origoScale = scale;
}

//void Transform::rotate(float amount, QVector3D axis)
//{
//    amount = 1.2f;
//    QMatrix4x4 temp{};
//    temp.rotate(amount, axis);
//    QVector4D tempRot(mLocalPosition);

//    tempRot = tempRot*temp;
//    mLocalPosition = QVector3D(tempRot);
//    qDebug() << mLocalRotation;
//}
