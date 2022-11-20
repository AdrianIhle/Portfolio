#ifndef ORFSCENE_H
#define ORFSCENE_H

#include "camera.h"
#include "cube.h"
#include "oktaederball.h"
#include "interactiveball.h"
#include "gameengine.h"
#include "gameobject.h"
#include "mesh.h"
#include "meshbase.h"
#include "skybox.h"
#include "axesgizmo.h"
#include <vector>
#include "material.h"


class ORFscene
{
public:
    ORFscene();
    ~ORFscene();
    void initScene();
    void drawScene(const QMatrix4x4 viewMatrix, const QMatrix4x4 projectionMatrix);

    std::vector<GameObject*> sceneGameObjects;
    GameObject* nSkybox;
    GameObject* nAxis;
    GameObject* nCircleSphere;

    void CreateSkybox();
    void CreateAxis();
    void CreateCircleSphere();
    void AddGameObject(GameObject *mObject);

    Material *nMaterials[4];

//    QString mName;

};

#endif // ORFSCENE_H
