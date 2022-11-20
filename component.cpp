#include "component.h"

Component::Component(GameObject *owner, QObject *parent) : QObject(parent)
{
    mOwner = owner;
}


void Component::setParentGameObject(GameObject *object)
{
    mOwner = object;
}

void Component::update()
{

}

GameObject *Component::getParentGameObject() const
{
    return mOwner;
}
