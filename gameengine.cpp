#include "gameengine.h"
#include "constants.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QtWidgets>
#include <QColorDialog>

#include <fstream>
#include <ostream>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>   //for sleep_for
#include <thread>   //for sleep_for

#include <cmath>
#include "makemesh.h"
#include "transform.h"
#include "audiosource.h"
#include "audiolistener.h"
#include "soundmanager.h"

GameEngine::GameEngine(QWidget *parent) : QOpenGLWidget(parent)
{
    //mGameEnginePointer = this;
}

GameEngine::~GameEngine()
{
    mRunning = false;
    mTimer.stop();
    cleanup();
}

void GameEngine::init()
{
    mRunning = true;

    initializeOpenGLFunctions();

    //Must set this to get MouseMoveEvents without pressing a mouse key
    //this->setMouseTracking(true);

    // Enable depth buffer - must be done in GameEngine
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling - must be done in GameEngine
    glEnable(GL_CULL_FACE);

    //Set up standard materials
    initMaterials();

    SoundManager::getInstance()->init();


    //Make and place camera
    mCamera = new Camera();
    mCamera->mTransform->setPosition(-0.0f, -0.0f, -0.0f);
    mCamera->setBackgroundColor(0.4f, 0.4f, 0.4f, 1.0f);
    mComponent = new AudioListener();
    mCamera->addComponent(mComponent);

    float mNearPlaneSize = 1.0f;
    float mFarPlaneSize = 100.0f;

    //Make and place camera
    mGameCamera = new Camera(800, 1200, mNearPlaneSize, mFarPlaneSize, 45.0f, QString("GameCamera"));
    mGameCamera->mTransform->setPosition(-3.0f, -2.0f, -10.0f);
    mGameCamera->setBackgroundColor(0.4f, 0.4f, 0.4f, 1.0f);
    mGameCamera->setName("GameCamera");
    mGameCamera->setMaterial(mMaterials[0]);
    mGeometry.push_back(mGameCamera); //kanskje denne



    //initializes object to identify the position of camera
//    GCobject = MakeMesh::makeMeshFromObj("Camera",mGameCamera->mTransform->getPosition().x,mGameCamera->mTransform->getPosition().y,mGameCamera->mTransform->getPosition().z, "GameCameraPos", "cam");
//    GCobject->setMaterial(mMaterials[0]);
//    GCobject->mTransform->setPosition(mGameCamera->mTransform->getPosition());
//    GCobject->setParent(mGameCamera);
//    mGeometry.push_back(GCobject);

    //Make and place camera
    mEditorCamera = new Camera();
    mEditorCamera->mTransform->setPosition(-3.0f, -2.0f, -50.0f);
    mEditorCamera->setBackgroundColor(0.4f, 0.4f, 0.4f, 1.0f);





    //player
    mPlayer = new InteractiveBall (2.0f,0.0f,2.0f,3, QString("player"));  //generates moveable ball
    mPlayer->setMaterial(mMaterials[0]);
    mGeometry.push_back(mPlayer);
    mSong = SoundManager::getInstance()->createSource(mPlayer, "Explosion", mPlayer->mTransform->getPosition(),
                                                      "../Sound/Assets/Caravan_mono.wav", true, 1.0f);
    mPlayer->addComponent(mSong);



    //Get the clear color from camera - the same as background color
    glClearColor(mCamera->mBackgroundColor.x(),
                 mCamera->mBackgroundColor.y(),
                 mCamera->mBackgroundColor.z(),
                 mCamera->mBackgroundColor.w());

    //Quick hack to make lots of cubes
    //The square root trick is because I want same number of rows and columns
    int noOfObjects{100};
    double squareRoot = sqrt(noOfObjects);
    GameObject *tempGeometry;
    for (unsigned long noOfCubesX = 0; noOfCubesX < squareRoot; noOfCubesX++)
    {
        for (unsigned long noOfCubesZ = 0; noOfCubesZ < squareRoot; noOfCubesZ++)
        {
            //tempGeometry = MakeMesh::makeCube((-3.0f+noOfCubesX*3.0f), 0.0f, (0.0f+noOfCubesZ*3.0f), QString("Cube "+ QString::number(mGeometry.size()+1)));
            tempGeometry = MakeMesh::makeMeshFromObj("Suzanne",(-3.0f+noOfCubesX*3.0f), 0.0f, (0.0f+noOfCubesZ*3.0f),"Suz", "Mankey");
            tempGeometry->setType("Mankey");
            tempGeometry->setMaterial(mMaterials[0]);
            mGeometry.push_back(tempGeometry);
        }
    }

//    //one textured cube:
//    tempGeometry = new Cube(2.0f, 2.0f, 0.0f, QString("Cube X"));
//    tempGeometry->setMaterial(mMaterials[1]);
//    tempGeometry->mTransform->setScale(2.0f, 2.0f, 2.0f);
//    mGeometry.push_back(tempGeometry);



//    tempGeometry = MakeMesh::makeCube(3.0f, 1.0f, -3.0f, "MyCube");
//    tempGeometry->setMaterial(mMaterials[0]);
//    mGeometry.push_back(tempGeometry);


    tempGeometry = MakeMesh::makeCube(0.0f,0.0f,0.0f, "script");

    tempScript = new ScriptComponent(tempGeometry, "ScriptMove.js");
    //need to use tempGeometry to access drawGeometry and stuff later
    tempGeometry->addComponent(tempScript);

    tempGeometry = MakeMesh::makeMeshFromObj("Suzanne",2,2,2,"Suz", "Mankey");
    tempGeometry->setType("Mankey");
    tempGeometry->setMaterial(mMaterials[0]);
    mGeometry.push_back(tempGeometry);

//    tempGeometry = MakeMesh::makeCube(4.0f, 2.0f, -4.0f,"TestChild");
//    tempGeometry->setMaterial(mMaterials[0]);
//    tempGeometry->setParent(mPlayer);
//    mGeometry.push_back(tempGeometry);
/*
    //          qDebug()<<mPlayer->mChildrenList[0]->getName() <<endl;

    //    tempGeometry = MakeMesh::makeMeshFromObj("testTextLoad.txt", 3.0f, 3.0f, 3.0f, "Suzanne");
    //    tempGeometry->setMaterial(mMaterials[0]);
    //    tempGeometry->mTransform->setScale(1,1,1);
    //    tempGeometry->mTransform->setPosition(0,0,0);
    //    mGeometry.push_back(tempGeometry);


    //plane
    //    mPlane = new Plane (-100.0f, -6.0f, -10.0f, "Ground Plane");
    //    mPlane->setMaterial(mMaterials[0]);
    //    mGeometry.push_back(mPlane);

    //    mPlane = new Plane(1.0f, 1.0f, 0.0f, 5, 2, QString("Plane"));
    //    mPlane->setMaterial(mMaterials[0]);
    //    mGeometry.push_back(mPlane);

    //    BarTrig = new BarycetricTriangulation(mPlane);
    //          foreach (auto g, mGeometry)
    //          {
    //              if(!g->mChildrenList.empty())
    //              {
    //              g->updateChildren();
    //              }
    //          }

*/
    mScene = new ORFscene();
    mScene->initScene();
    mScene->sceneGameObjects = mGeometry;

/*
    //    //circle-test:
    //    CircleSphere *tempCircle = new CircleSphere(4, -2.0f, 3.0f, -3.0f); //inferior stopped, signal from operating system, segmentation fault, error contiunes in new_allocator.h line 104 (return static cast)
    //    tempCircle->setMaterial(mMaterials[0]);
    //    mGeometry.push_back(tempCircle);

    //    //Axes - cross gizmo:
    //    axes = new AxesGizmo();
    //    axes->setMaterial(mMaterials[0]);


    //    skybox = new SkyBox();
    //    skybox->setMaterial(mMaterials[1]);
    //    skybox->mTransform->setScale(150.0f, 150.0f, 150.0f);
    //    mGeometry.push_back(skybox);

*/
    //    // Starts the Game Loop
    //    // Use QBasicTimer because its faster than QTimer
    mTimer.start(16, this);
        mTimeStart.start();

    //send signal to Hierarcy in GUI
    initHierarchy(mGeometry);



}

void GameEngine::setCameraColor()
{
    QColor color = QColor(static_cast<int>(mCamera->mBackgroundColor.x()*255),
                          static_cast<int>(mCamera->mBackgroundColor.y()*255),
                          static_cast<int>(mCamera->mBackgroundColor.z()*255),
                          static_cast<int>(mCamera->mBackgroundColor.w()*255));

    QString title = QString("Select Background color");
    color = QColorDialog::getColor(color, this, title);

    if (color.isValid())
    {
        mCamera->setBackgroundColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());

        glClearColor(mCamera->mBackgroundColor.x(),
                     mCamera->mBackgroundColor.y(),
                     mCamera->mBackgroundColor.z(),
                     mCamera->mBackgroundColor.w());
    }
}

void GameEngine::initMaterials()
{
    // Makes 4 default materials
    // 0.UnlitColor  1.UnlitTexture 2.LitColors 3.LitTexture
    // Only UnlitColor and UnlitTexture is implemented for now

    for (int no = 0; no < 4; no++)
    {
        mMaterials[no] = new Material(no);
    }
}

void GameEngine::cleanup()
{
    //Delete what we made in init()
    //    qDebug() << "Cleanup!";
    //    qDebug() << "Deleting" << mGeometry.size() << "objects";

    for (unsigned long noOfObjects = 0; noOfObjects < mScene->sceneGameObjects.size(); ++noOfObjects)
    {
        delete mScene->sceneGameObjects.at(noOfObjects);

        //The mGeometry vector deletes itself when going out of scope
    }
    mScene->sceneGameObjects.clear();
    //    qDebug() << "mGeometry is now " << mGeometry.size() << "objects";

    if (mScene->nAxis)
    {
        delete mScene->nAxis;
        mScene->nAxis = nullptr;
        //        qDebug() << "Axes deleted";
    }
    for (int no = 0; no < 4; no++)
    {
        delete mMaterials[no];
        mMaterials[no] = nullptr;
        //        qDebug() << "Material" << no << "deleted";
    }

    if(mCamera)
    {
        delete mCamera;
        mCamera = nullptr;
        //        qDebug() << "Camera deleted";
    }

    mScene->~ORFscene();
}

//This function is called from the Qt framework
void GameEngine::initializeGL()
{
    initializeOpenGLFunctions();
    init();
}

void GameEngine::resizeGL(int w, int h)
{
    //This function is called from Qt
    //Maybe before the camera is made!

    //The following is sometimes needed for retina displays
    //    const qreal retinaScale = devicePixelRatio();
    //    qDebug() << "Retina " << retinaScale;

    mCamera->setAspectRatio(w, h);
}

void GameEngine::paintGL()
{
    qDebug() << "frame start" << endl;
    verticesDrawn = 0;
    //paintGL is the Qt function called from the internal Qt loop

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(mCamera->mBackgroundColor.x(),
                 mCamera->mBackgroundColor.y(),
                 mCamera->mBackgroundColor.z(),
                 mCamera->mBackgroundColor.w());

    if(editorCamera){
        mCamera = mEditorCamera;

    }
    else
    {
        mCamera = mGameCamera;
    }

    // Calculate view transformation
    if (!mCamera)
        qDebug() << "Rendering without camera!";

    // cameras viewMatrix - same for all objects
    viewMatrix = mCamera->getMatrix();
    // cameras projectionMatrix - same for all objects
    projectionMatrix = mCamera->getPerspectiveMatrix();

    //set position of skybox to that of camera
    //Not 100% correct yet.
    Vec3 temp = mCamera->mTransform->getPosition();
    temp.z = -temp.z;
    temp.x = -temp.x;
    mScene->nSkybox->mTransform->setPosition(temp);

    mGameCamera->setViewMatrix(viewMatrix);
    mGameCamera->setprojectionMatrix(projectionMatrix);
//    mGameCamera->drawGeometry();


    foreach (auto &tempGameObject, mGeometry) {
        tempGeometry = tempGameObject;
        tempGeometry->setViewMatrix(viewMatrix);
        tempGeometry->setprojectionMatrix(projectionMatrix);
        //lod level selection test:
        if(mGameCamera!= nullptr && (mGameCamera->insideFrustum(tempGeometry->mTransform->getPosition(), tempGeometry->getRadius())))
        {
            //lod level selection test:


                if (tempGeometry->getType() == "Mankey") //only mesh with lod...
                {
                    qDebug() << tempGeometry->getType();
                    Vec3 temp = mGameCamera->mTransform->getPosition();
                    temp = temp - tempGeometry->mTransform->getPosition();
                    float tempLength = temp.length();
                    qDebug()<<temp.length();
                    if (tempLength < 20){
                        static_cast<Mesh*>(tempGeometry)->setLodLevel(0);//sier at tempGeometry er nå en type av mesh, som kan få funksjoner derfra
                    }
                    else if (tempLength < 40){
                        static_cast<Mesh*>(tempGeometry)->setLodLevel(1);
                    }
                    else if (tempLength < 60){
                        static_cast<Mesh*>(tempGeometry)->setLodLevel(2);
                    }
                    qDebug() << "mankey drawn" <<endl;
                }


                verticesDrawn += tempGeometry->drawGeometry();
            }
            /*
            else if(tempGameObject->getName() == "Mankey") //only mesh with lod...
            {
                static_cast<mesh*>(tempGeometry)->setLoadLevel(lodlevel);
            }*/


//            tempGeometry->drawGeometry();

        /*
           else if(tempGameObject->getName() == "Mankey") //only mesh with lod...
           {
               static_cast<mesh*>(tempGeometry)->setLoadLevel(lodlevel);
           }*/

    }
/*
//    mScene->drawScene(viewMatrix, projectionMatrix);

    //Draw all geometry in mGeometry vector:
    //    GameObject *tempGeometry;

    //    foreach (auto &tempGameObject, mGeometry) {
    //        tempGeometry = tempGameObject;
    //        tempGeometry->setViewMatrix(viewMatrix);
    //        tempGeometry->setprojectionMatrix(projectionMatrix);
    //        tempGeometry->drawGeometry();
    //    }



    //        GCobject->setViewMatrix(viewMatrix);
    //        GCobject->setprojectionMatrix(projectionMatrix);
    //        GCobject->drawGeometry();

    //if Axis editor button is on:
//    if (axisOn)
//    {
//        //                mScene->nAxis->setViewMatrix(viewMatrix);
//        //                mScene->nAxis->setprojectionMatrix(projectionMatrix);
//        //                mScene->nAxis->drawGeometry();
//    }
*/

}

void GameEngine::axisOnOff()
{
    // qDebug() << "Axis on/off";
    axisOn = !axisOn;
}

void GameEngine::orthographicOnOff()
{
    ortho = !ortho;
    if (ortho)
        mCamera->setAspectRatio(1280, 720, true);
    else
        mCamera->setAspectRatio(1280, 720);
}

void GameEngine::wireFrameOnOff()
{
    //qDebug() << "Wireframe on/off";
    wireFrame = !wireFrame;
    GameObject *tempGeometry;
    foreach (auto &tempGameObject, mGeometry) {
        tempGeometry = tempGameObject;
        tempGeometry->setRendermode(wireFrame);
    }
}

void GameEngine::UpdateChild(GameObject* child, GameObject* parent)
{
    child->mTransform->setPosition(parent->mTransform->getPosition());
    child->mTransform->setRotation(parent->mTransform->getRotation());

}

void GameEngine::timerEvent(QTimerEvent *e)
{

    foreach(GameObject* GO, mScene->sceneGameObjects)
    {
        if(GO->mType == "Plane")
        {
            Plane* p = dynamic_cast<Plane*>(GO);
            BarTrig = new BarycetricTriangulation(p);
            Vec2* mPlayerPosition = new Vec2(mPlayer->mTransform->getPosition().x,mPlayer->mTransform->getPosition().z);
            BarTrig->HighlightTriangle(mPlayerPosition);
        }
    }
    //just to silence warnings:
    //should be fixed - now just using time for nothing
    if(false)
        qDebug() << e;

    Vec3 newdir(0.2f,0.0f,0.f);
    mSong->setPosition(mPlayer->mTransform->getPosition());
    tempScript->update();
    handleKeys();
//    UpdateChild(GCobject, mGameCamera);

    foreach (auto g, mGeometry)
    {
        if(!g->mChildrenList.empty())
        {
            g->updateChildren();
        }
    }


    // Request an update for the QOpenGL widget
    update();

    msSinceLastFrame = mTimeStart.restart();

    static int frameCount{0};
    ++frameCount;
    if (frameCount > 60)
    {
//        EditorWindow* mEditor{nullptr};
//        mEditor->statusBar()->showMessage("Vertices: " + QString::number(verticesDrawn)
//                                          + ", FrameDraw in ms: " + QString::number(msSinceLastFrame));
        frameCount = 0;
    }


}

int GameEngine::getFrameRate()
{
    return 1000/msSinceLastFrame;
}

void GameEngine::mouseMoveEvent(QMouseEvent *event)
{
    //using mouseXYlast as deltaXY so we don't need extra variables
    mouseXlast = event->pos().x() - mouseXlast;
    mouseYlast = event->pos().y() - mouseYlast;

    //if delta is to big the movement will be jerky
    //Happens if mouse is moved much between presses.
    if (mouseXlast > 40 || mouseYlast > 40 || mouseXlast < -40 || mouseYlast < -40)
    {
        mouseXlast = 0;
        mouseYlast = 0;
    }

    //qDebug() << "dX: "<< mouseXlast << ", dY: "<< mouseYlast;
    mCamera->mTransform->rotate(0.0f,cameraSpeed*mouseXlast,0.0f);
    mCamera->mTransform->rotate(cameraSpeed*mouseYlast,0.0f,0.0f);

    mouseXlast = event->pos().x();
    mouseYlast = event->pos().y();
}

void GameEngine::keyPressEvent(QKeyEvent *event)
{
    //move camera
    if(event->key() == Qt::Key_A)
    {
        mLeft = true;
    }
    if(event->key() == Qt::Key_D)
    {
        mRight  = true;
    }
    if(event->key() == Qt::Key_W)
    {
        mUp = true;
    }
    if(event->key() == Qt::Key_S)
    {
        mDown = true;
    }

    //    moves player based on input, moves in world space, not local
    switch (event->key()) {
    case Qt::Key_Up:
        mPlayer->mTransform->translate(0.0f, 0.0f, -0.2f);

        break;
    case Qt::Key_Left:
        mPlayer->mTransform->translate(-0.2f, 0.0f, 0.0f);
        break;
    case Qt::Key_Down:
        mPlayer->mTransform->translate(0.0f, 0.0f, 0.2f);
        break;
    case Qt::Key_Right:
        mPlayer->mTransform->translate(0.2f, 0.0f, 0.0f);
        break;
    case Qt::Key_0:
        //        dynamic_cast<player*>(m_player)->getV()->m_degrees += 5.0;
        break;
    default:
        break;
    }
}

void GameEngine::keyReleaseEvent(QKeyEvent *event)
{
    //move camera
    if(event->key() == Qt::Key_A)
    {
        mLeft = false;
    }
    if(event->key() == Qt::Key_D)
    {
        mRight  = false;
    }
    if(event->key() == Qt::Key_W)
    {
        mUp = false;
    }
    if(event->key() == Qt::Key_S)
    {
        mDown = false;
    }
}

void GameEngine::wheelEvent(QWheelEvent* event)
{
    //http://doc.qt.io/qt-4.8/qwheelevent.html
    int numDegrees = event->delta() / 8;
    float numSteps = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal) {
        mCamera->mTransform->translate(numSteps, 0.0, 0.0);
    } else {
        mCamera->mTransform->translate(0.0, 0.0, numSteps);
    }
    event->accept();
}

void GameEngine::handleKeys()
{
    //move camera
    if(mLeft)
    {
        mCamera->mTransform->translate(0.2f, 0.0, 0.0);

    }
    if(mRight)
    {
        mCamera->mTransform->translate(-0.2f, 0.0, 0.0);
    }
    if(mUp)
    {
        mCamera->mTransform->translate(0.0, 0.0, 0.2f);
    }
    if(mDown)
    {
        mCamera->mTransform->translate(0.0, 0.0, -0.2f);
    }
    if(!editorCamera)
    {
        mCamera = mGameCamera;
    }
}

void GameEngine::addCube()
{
    Cube *tCube = new Cube(2,2,2,"new Cube");
    tCube->setMaterial(mMaterials[0]);
    mScene->AddGameObject(tCube);
    initHierarchy(mScene->sceneGameObjects);
}

void GameEngine::addBall()
{
    OktaederBall *tBall = new OktaederBall(4, 1, 2, 3, "new ball");
    tBall->setMaterial(mMaterials[0]);
    mScene->AddGameObject(tBall);
    initHierarchy(mScene->sceneGameObjects);
}

void GameEngine::addPlane()
{
    Plane *tPlane = new Plane(0,3,0,5,5,"new Plane");
    tPlane->setMaterial(mMaterials[0]);
    mScene->AddGameObject(tPlane);
    initHierarchy(mScene->sceneGameObjects);
}

void GameEngine::readFromFile(std::string filePath, QString objName)
{
    filePath = nullptr;
    //    GameObject* makeNewMesh = MakeMesh::makeMeshFromObj(filePath, 1.0f,1.0f,1.0f, objName);
    GameObject* newMesh = MakeMesh::makeCube(1.1f,1.1f,-1.1f, objName);
    newMesh->setMaterial(mMaterials[0]);
    mScene->AddGameObject(newMesh);
    initHierarchy(mScene->sceneGameObjects);

}

GameObject* GameEngine::ObjectFromList(int listIndex)
{
    return mScene->sceneGameObjects[listIndex];
}

void GameEngine::SaveScene(QString filePath, ORFscene* mScene)
{

    std::string filename = filePath.toStdString();
    std::ofstream out (filename.c_str());




    std::vector<GameObject*> &sceneObjects = mScene->sceneGameObjects;
    Transform* tTransform;



    out << "#SceneStart" << endl;
    out << "\n" << "#GameObjects" << " " << std::endl;

    for(unsigned int i = 0; i < sceneObjects.size(); i++)
    {
        out << "#New"<< "\n";

        out << "-name" << " " << sceneObjects[i]->getName().toStdString() << endl
            <<"-type " << sceneObjects[i]->mType.toStdString() << "\n";

        tTransform = sceneObjects[i]->mTransform;
        Vec3 position = tTransform->getPosition();
        Vec3 rotation = tTransform->getRotation();
        Vec3 scale = tTransform->getScale();

        out << "-position" << " " << position.x << " , " << position.y << " , " << position.z << endl
            << "-rotation" <<" " << rotation.x << " , " << rotation.y << " , " << rotation.z << endl
            << "-scale" << " " << scale.x << " , " << scale.y << " , " << scale.z << endl
            <<"-material" << " " << "0"<< endl
           <<"#End"<<endl;


    }
    tTransform = nullptr;
    out.close();




}

void GameEngine::LoadScene(std::string filepath, ORFscene* newScene)
{
    std::string mFileName;
    std::string mOneLine;
    std::string mOneWord;

    mFileName = filepath;

    std::ifstream mFileIn(mFileName);


    while(getline(mFileIn, mOneLine)) //not working
    {
        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << mOneLine;
        //Streaming one word out of line
        sStream >> mOneWord;

        GameObject* temp ;
        if (mOneWord == "#New")
        {
            temp = nullptr;
        }


        if (mOneWord == "-type")
        {
            sStream >> mOneWord;
            if(mOneWord == "Camera")
            {
                temp = new Camera();
                sStream >> mOneWord;
                if(mOneWord == "-position")
                {
                    Vec3 pos;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            pos.x = std::stof(mOneWord);
                        if(i == 1)
                            pos.y = std::stof(mOneWord);
                        if(i == 2)
                            pos.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setPosition(pos);
                }

                sStream >> mOneWord;
                if(mOneWord == "-rotation")
                {
                    Vec3 rot;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            rot.x = std::stof(mOneWord);
                        if(i == 1)
                            rot.y = std::stof(mOneWord);
                        if(i == 2)
                            rot.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(rot);
                }

                sStream >> mOneWord;
                if(mOneWord == "-scale")
                {
                    Vec3 sca;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            sca.x = std::stof(mOneWord);
                        if(i == 1)
                            sca.y = std::stof(mOneWord);
                        if(i == 2)
                            sca.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(sca);
                }

                sStream >> mOneWord;
                if(mOneWord == "-material")
                {
                    sStream >> mOneWord;
                    temp->setMaterial(mMaterials[std::stoi(mOneWord)]);
                }

                mScene->sceneGameObjects.push_back(temp);
            }

            if(mOneWord == "CircleSphere")
            {
                temp = new CircleSphere();
                sStream >> mOneWord;
                if(mOneWord == "-position")
                {
                    Vec3 pos;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            pos.x = std::stof(mOneWord);
                        if(i == 1)
                            pos.y = std::stof(mOneWord);
                        if(i == 2)
                            pos.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setPosition(pos);
                }

                sStream >> mOneWord;
                if(mOneWord == "-rotation")
                {
                    Vec3 rot;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            rot.x = std::stof(mOneWord);
                        if(i == 1)
                            rot.y = std::stof(mOneWord);
                        if(i == 2)
                            rot.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(rot);
                }

                sStream >> mOneWord;
                if(mOneWord == "-scale")
                {
                    Vec3 sca;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            sca.x = std::stof(mOneWord);
                        if(i == 1)
                            sca.y = std::stof(mOneWord);
                        if(i == 2)
                            sca.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(sca);
                }

                sStream >> mOneWord;
                if(mOneWord == "-material")
                {
                    sStream >> mOneWord;
                    temp->setMaterial(mMaterials[std::stoi(mOneWord)]);
                }
                mScene->sceneGameObjects.push_back(temp);
            }
            if(mOneWord == "Cube")
            {
                temp = new Cube();

                sStream >> mOneWord;
                if(mOneWord == "-position")
                {
                    Vec3 pos;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            pos.x = std::stof(mOneWord);
                        if(i == 1)
                            pos.y = std::stof(mOneWord);
                        if(i == 2)
                            pos.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setPosition(pos);
                }

                sStream >> mOneWord;
                if(mOneWord == "-rotation")
                {
                    Vec3 rot;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            rot.x = std::stof(mOneWord);
                        if(i == 1)
                            rot.y = std::stof(mOneWord);
                        if(i == 2)
                            rot.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(rot);
                }

                sStream >> mOneWord;
                if(mOneWord == "-scale")
                {
                    Vec3 sca;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            sca.x = std::stof(mOneWord);
                        if(i == 1)
                            sca.y = std::stof(mOneWord);
                        if(i == 2)
                            sca.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(sca);
                }

                sStream >> mOneWord;
                if(mOneWord == "-material")
                {
                    sStream >> mOneWord;
                    temp->setMaterial(mMaterials[std::stoi(mOneWord)]);
                }

                mScene->sceneGameObjects.push_back(temp);

            }
            if(mOneWord == "InteractiveBall")
            {
                temp = new InteractiveBall();

                sStream >> mOneWord;
                if(mOneWord == "-position")
                {
                    Vec3 pos;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            pos.x = std::stof(mOneWord);
                        if(i == 1)
                            pos.y = std::stof(mOneWord);
                        if(i == 2)
                            pos.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setPosition(pos);
                }

                sStream >> mOneWord;
                if(mOneWord == "-rotation")
                {
                    Vec3 rot;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            rot.x = std::stof(mOneWord);
                        if(i == 1)
                            rot.y = std::stof(mOneWord);
                        if(i == 2)
                            rot.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(rot);
                }

                sStream >> mOneWord;
                if(mOneWord == "-scale")
                {
                    Vec3 sca;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            sca.x = std::stof(mOneWord);
                        if(i == 1)
                            sca.y = std::stof(mOneWord);
                        if(i == 2)
                            sca.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(sca);
                }

                sStream >> mOneWord;
                if(mOneWord == "-material")
                {
                    sStream >> mOneWord;
                    temp->setMaterial(mMaterials[std::stoi(mOneWord)]);
                }
                mScene->sceneGameObjects.push_back(temp);

            }
            if(mOneWord == "Generic Mesh")
                qDebug() << "cannot gerneate mesh"<<endl;

            if(mOneWord == "Ball")
            {
                temp = new OktaederBall();
                sStream >> mOneWord;
                if(mOneWord == "-position")
                {
                    Vec3 pos;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            pos.x = std::stof(mOneWord);
                        if(i == 1)
                            pos.y = std::stof(mOneWord);
                        if(i == 2)
                            pos.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setPosition(pos);
                }

                sStream >> mOneWord;
                if(mOneWord == "-rotation")
                {
                    Vec3 rot;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            rot.x = std::stof(mOneWord);
                        if(i == 1)
                            rot.y = std::stof(mOneWord);
                        if(i == 2)
                            rot.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(rot);
                }

                sStream >> mOneWord;
                if(mOneWord == "-scale")
                {
                    Vec3 sca;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            sca.x = std::stof(mOneWord);
                        if(i == 1)
                            sca.y = std::stof(mOneWord);
                        if(i == 2)
                            sca.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(sca);
                }

                sStream >> mOneWord;
                if(mOneWord == "-material")
                {
                    sStream >> mOneWord;
                    temp->setMaterial(mMaterials[std::stoi(mOneWord)]);
                }
                mScene->sceneGameObjects.push_back(temp);

            }
            if(mOneWord == "Skybox")
            {
                temp = new SkyBox();
                sStream >> mOneWord;
                if(mOneWord == "-position")
                {
                    Vec3 pos;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            pos.x = std::stof(mOneWord);
                        if(i == 1)
                            pos.y = std::stof(mOneWord);
                        if(i == 2)
                            pos.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setPosition(pos);
                }

                sStream >> mOneWord;
                if(mOneWord == "-rotation")
                {
                    Vec3 rot;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            rot.x = std::stof(mOneWord);
                        if(i == 1)
                            rot.y = std::stof(mOneWord);
                        if(i == 2)
                            rot.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(rot);
                }

                sStream >> mOneWord;
                if(mOneWord == "-scale")
                {
                    Vec3 sca;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            sca.x = std::stof(mOneWord);
                        if(i == 1)
                            sca.y = std::stof(mOneWord);
                        if(i == 2)
                            sca.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(sca);
                }

                sStream >> mOneWord;
                if(mOneWord == "-material")
                {
                    sStream >> mOneWord;
                    temp->setMaterial(mMaterials[std::stoi(mOneWord)]);
                }
                mScene->sceneGameObjects.push_back(temp);

            }

            if(mOneWord == "GameCameraPos")
            {
                GCobject = MakeMesh::makeMeshFromObj("Camera",mGameCamera->mTransform->getPosition().x,mGameCamera->mTransform->getPosition().y,mGameCamera->mTransform->getPosition().z, "GameCameraPos","cam");
                sStream >> mOneWord;
                if(mOneWord == "-position")
                {
                    Vec3 pos;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            pos.x = std::stof(mOneWord);
                        if(i == 1)
                            pos.y = std::stof(mOneWord);
                        if(i == 2)
                            pos.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setPosition(pos);
                }

                sStream >> mOneWord;
                if(mOneWord == "-rotation")
                {
                    Vec3 rot;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            rot.x = std::stof(mOneWord);
                        if(i == 1)
                            rot.y = std::stof(mOneWord);
                        if(i == 2)
                            rot.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(rot);
                }

                sStream >> mOneWord;
                if(mOneWord == "-scale")
                {
                    Vec3 sca;
                    for(int i = 0; i < 3; i++)
                    {
                        sStream >> mOneWord;
                        if(i == 0)
                            sca.x = std::stof(mOneWord);
                        if(i == 1)
                            sca.y = std::stof(mOneWord);
                        if(i == 2)
                            sca.z = std::stof(mOneWord);
                    }
                    temp->mTransform->setRotation(sca);
                }

                sStream >> mOneWord;
                if(mOneWord == "-material")
                {
                    sStream >> mOneWord;
                    temp->setMaterial(mMaterials[std::stoi(mOneWord)]);
                }

                mScene->sceneGameObjects.push_back(temp);

            }

            newScene->nMaterials[0] = mMaterials[0];
            GameObject* temp = nullptr;
            temp->getName();
        }


        if (mOneWord == "#End")
        {
            continue;
        }

        newScene = mScene;
    }

    mFileIn.close();
    mScene->initScene();
}
