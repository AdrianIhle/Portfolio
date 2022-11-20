#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QBasicTimer>
#include <QTIme>

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>



#include <vector>

#include "camera.h"
#include "axesgizmo.h"
#include "skybox.h"
#include "cube.h"
#include "circlesphere.h"
#include "shaderprogram.h"
#include "plane.h"
#include "interactiveball.h"
#include "barycetrictriangulation.h"
#include "oktaederball.h"
#include "orfscene.h"
#include "sosiplane.h"
#include "component.h"
#include "audiosource.h"
#include "scriptcomponent.h"
#include "mesh_file.h"

class ORFscene;

class GameEngine : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GameEngine(QWidget *parent = 0);
    ~GameEngine();
    void setLodLevel(int lod);
    void init();
    void initMaterials();
    //void initTextures();
    void cleanup();


    bool mPlay = false;
    bool camin = false;
    bool running() { return mRunning; }
    bool editorCamera = false;
    void quit() { mRunning = false; }
    BarycetricTriangulation *BarTrig;
    void addBall();
    void addCube();
    void addPlane();
    void readFromFile(std::string filePath, QString objName);
    void SaveScene(QString filePath, ORFscene *mScene);
    void LoadScene(std::string filepath, ORFscene *newScene);
    void  UpdateChild(GameObject* child, GameObject* parent);
    GameObject *ObjectFromList(int listIndex);
    unsigned int verticesDrawn{};
    Component* tempScript;

    ORFscene* mScene;
    int getFrameRate();

signals:
    void initHierarchy(std::vector<GameObject*> &mGeometry);

public slots:
    void axisOnOff();
    void wireFrameOnOff();
    void setCameraColor();
    void orthographicOnOff();

protected:
    //This is the "handleEvents" part:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

    void handleKeys();

    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    Plane* mPlane;
    GameObject *mPlayer;
    GameObject* mGravBall;
    Vec2* mPlayerPosition;
    Component* mComponent;
    AudioSource* mSong;
private:

    int msSinceLastFrame;
    QBasicTimer mTimer;
    QTime mTimeStart;

    Material *mMaterials[4];

    GameObject* mFrut;
    Mesh_file* mesh_file;

    Camera* mCamera;
    Camera *mGameCamera;
    Camera *mEditorCamera;

    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;

    std::vector<GameObject*> mGeometry;

    //    GameObject *axes;
    //    GameObject *skybox;
    GameObject *GCobject;
    GameObject* tempGeometry;

    QString* fragmentShaderFileName;
    QString* vertexShaderFileName;

    bool mRunning;

    bool axisOn = true;
    bool wireFrame = false;
    bool ortho = false;

    int mouseXlast{};
    int mouseYlast{};
    float cameraSpeed = 0.2f;

    //This really! should be made anoter way!
    //Quick hack ahead:
    bool mW = false;
    bool mA = false;
    bool mS = false;
    bool mD = false;
    bool mUp = false;
    bool mDown = false;
    bool mLeft = false;
    bool mRight = false;
    bool mQ = false;
    bool mE = false;

    void startOpenGLDebugger();
    int lodlevel{-1};

};

#endif // GAMEENGINE_H
