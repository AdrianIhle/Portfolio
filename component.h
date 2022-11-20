#ifndef COMPONENT_H
#define COMPONENT_H
#include "gameobject.h"
#include <QObject>

class GameObject;

class Component : public QObject
{
public:
    Component(GameObject* owner, QObject* parent = nullptr);
    virtual void update();
    GameObject* getParentGameObject() const;
    void setParentGameObject(GameObject* object);

protected:
    GameObject* mOwner;
};

#endif // COMPONENT_H
