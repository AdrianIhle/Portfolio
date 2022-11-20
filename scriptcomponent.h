#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H
#include "gameobject.h"
#include "component.h"
#include <QJSEngine>

class GameObject;

class ScriptComponent :public Component
{
    Q_OBJECT
public:
    ScriptComponent(GameObject* owner, QString scriptFileNameIn);
    virtual void update() override;
    Q_INVOKABLE void move();
private:
    void initScriptFile();
    QJSEngine mJSEngine;
    QString mScriptFileName;

};

#endif // SCRIPTCOMPONENT_H
