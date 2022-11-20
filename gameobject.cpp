#include "gameobject.h"
#include "vec3.h"
#include "transform.h"

#include <QVector2D>
#include <QVector3D>


GameObject::GameObject(float xPos, float yPos, float zPos, QString nameIn, QString type) :mParent(nullptr)
{
    initializeOpenGLFunctions();

    mTransform = new Transform(this);
	mTransform->setPosition(Vec3(xPos, yPos, zPos));
	name = nameIn;
    mType = type;
}

GameObject::~GameObject()
{
    delete mTransform;
}

void GameObject::setParent(GameObject *parent)
{
    mParent = parent;
    mTransform->setOrigo(mParent->mTransform->getPosition()
                         ,mParent->mTransform->getRotation()
                         ,mParent->mTransform->getScale());
    mParent->mChildrenList.push_back(this);
    //Do something to my own transform?
}

void GameObject::setViewMatrix(QMatrix4x4 viewMatrix)
{
	mViewMatrix = viewMatrix;
}

void GameObject::setprojectionMatrix(QMatrix4x4 projectionMatrix)
{
	mProjectionMatrix = projectionMatrix;
}

void GameObject::setName(QString nameIn)
{
    name = nameIn;
}

void GameObject::addComponent(Component* inComponent)
{
    mComponents.push_back(inComponent);
}

QString GameObject::getName()
{
	return name;
}

void GameObject::setType(const QString &type)
{
    mType = type;
}

QString GameObject::getType() const
{
    return mType;
}

QMatrix4x4 GameObject::getMatrix()
{
    mModelMatrix.setToIdentity();
    mModelMatrix.translate(mTransform->getPosition().x, mTransform->getPosition().y, mTransform->getPosition().z );
    mModelMatrix.rotate(mTransform->getRotation().x, 1.0, 0.0, 0.0);
    mModelMatrix.rotate(mTransform->getRotation().y, 0.0, 1.0, 0.0);
    mModelMatrix.rotate(mTransform->getRotation().z, 0.0, 0.0, 1.0);
    mModelMatrix.scale(mTransform->getScale().x, mTransform->getScale().y, mTransform->getScale().z);
    //mModelMatrix.translate(mTransform.mPosition);
    return mModelMatrix;
}

int GameObject::initGeometry(Material* m)
{
    this->setMaterial(m);
    return 0;
}

float GameObject::getRadius() const
{
    return mRadius*mTransform->getScale().length();
}

void GameObject::updateChildren()
{
    foreach(auto g, mChildrenList)
    {
        g->updateFromParent();
    }
}

void GameObject::updateFromParent()
{

    mTransform->setOrigo(mParent->mTransform->getPosition()
                         ,mParent->mTransform->getRotation()
                         ,mParent->mTransform->getScale());

}

void GameObject::initChildren()
{
    foreach(auto g, mChildrenList)
    {
        g->initGeometry(g->mMaterial);
    }
}

void GameObject::drawChildren()
{
    foreach(auto g, mChildrenList)
    {
        g->drawGeometry();
    }
}


