#include "orfscene.h"
#include "sosiplane.h"

ORFscene::ORFscene()
{
    CreateSkybox();
    CreateAxis();
    CreateCircleSphere();

}

ORFscene::~ORFscene()
{
    for(unsigned int i = 0; i < sceneGameObjects.size(); i++)
    {
        delete sceneGameObjects[i];
        sceneGameObjects[i] = nullptr;
    }
    sceneGameObjects.clear();

    delete nSkybox;
    nSkybox = nullptr;
    delete nAxis;
    nAxis = nullptr;
    delete nCircleSphere;
    nCircleSphere = nullptr;


}

void ORFscene::AddGameObject(GameObject* mObject)
{
    sceneGameObjects.push_back(mObject);
}

void ORFscene::initScene()
{
    foreach(auto object, sceneGameObjects)
    {
        object->initGeometry(nMaterials[0]);
    }

//    nSkybox->initGeometry(nMaterials[0]);
//    nAxis->initGeometry(nMaterials[0]);
//    nCircleSphere->initGeometry(nMaterials[0]);

}

void ORFscene::drawScene(const QMatrix4x4 viewMatrix, const QMatrix4x4 projectionMatrix)
{

    foreach(auto *object, sceneGameObjects)
    {
        object->setViewMatrix(viewMatrix);
        object->setprojectionMatrix(projectionMatrix);
        object->drawGeometry();
    }

//    nSkybox->drawGeometry();
//    nAxis->drawGeometry();
//    nCircleSphere->drawGeometry();

}


void ORFscene::CreateSkybox()
{
    nSkybox = new SkyBox();
    nSkybox->setMaterial(nMaterials[1]);
    nSkybox->mTransform->setScale(150.0f, 150.0f, 150.0f);
    sceneGameObjects.push_back(nSkybox);
}

void ORFscene::CreateAxis()
{
    nAxis = new AxesGizmo();
    nAxis ->setMaterial(nMaterials[0]);
}

void ORFscene::CreateCircleSphere()
{
    nCircleSphere = new CircleSphere(4, -2.0f, 3.0f, -3.0f); //inferior stopped, signal from operating system, segmentation fault, error contiunes in new_allocator.h line 104 (return static cast)
    nCircleSphere->setMaterial(nMaterials[0]);
}
