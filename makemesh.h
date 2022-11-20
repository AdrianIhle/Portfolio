#ifndef MAKEMESH_H
#define MAKEMESH_H

#include <QString>
#include "camera.h"

class GameObject;

class MakeMesh
{
public:
    static GameObject* makeCube(float xPos, float yPos, float zPos, QString objectName);
    static GameObject* makeMeshFromObj(std::string fileName, float xPos, float yPos, float zPos, QString objectName, QString type);
private:
    MakeMesh();
};

#endif // MAKEMESH_H
