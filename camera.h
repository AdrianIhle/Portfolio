#ifndef CAMERA_H
#define CAMERA_H

#include <QVector4D>
#include <QMatrix4x4>
#include "transform.h"
#include "mesh.h"
#include "gameobject.h"
//#include "Mesh_vertexbuffer.h"
#include "BoundingVolume.h"
#include <vector>
//#include "Mesh_file.h"


class Camera : public GameObject
{
public:
    Camera(int height = 800, int width = 1200, float nearPlane = 1.0f, float farPlane = 400.0f, float fov = 45.0f, QString name = "GameCamera");
    void rotate(float angle, const QVector3D &axis);
    void translate (float x, float y, float z);
    void setAspectRatio(int w, int h, bool ortho = false);
    void setBackgroundColor(float r, float g, float b, float a);
    void invertCam();
    //void frustrum(/*Camera *cameraIn, int xPos = 0, int yPos = 0, int zPos = 0, QString name = "Frustum"*/);
    //bool frustumClipping(Camera* cameraIn, GameObject* curGameObject, Mesh_file* meshfile);

   // Vec3 getFrustrum();

    unsigned int drawGeometry();
    int initGeometry();
    //for compability with GameObject
    //will be used when main camera is visible in editor
    //qDebug-stuff just to silence warnings...
    void setMaterial(Material *materialIn);
    void setRendermode(int mode);

    QMatrix4x4 getPerspectiveMatrix() {return mPerspectiveMatrix;}
    QMatrix4x4 worldToViewMatrix();

  //  QMatrix4x4 getViewMatrix(){return getMatrix();/*worldToViewMatrix();*/}
    //QMatrix4x4 getMatrix();

    QVector4D mBackgroundColor;

    //GameObject* getFrustrum() const;



    bool insideFrustum(Vec3 pos, float radius);

    void bary(Vec3 p, Vec3 v0, Vec3 v1, Vec3 v2, GLfloat &u, GLfloat &v, GLfloat &w, int axes);
    //float getNearPlane(){mNearPlane;}
    //float getFarPlane(){mFarPlane;}

    //can be moved anywhere, if this doesn't work. Might be good to move into meshfile or gamepbject
    //char getVertexSideOfPlaneNormal(Vec3 planeNormal, Vec3 vertexPos, Vec3 origin);

private:

    //forward declaration
    //class Frustum* fru;

    QMatrix4x4 mView;

    void setFrustrum();

    Vec3 mPosition;
    Vec3 mForward;//(0,0,-1);
    Vec3 mUp;
    Vec3 mFrustumFarPoints[3];
    GameObject* mFrustrum;
    QString mObjectname;
    float mNearPlane;
    float mFarPlane;
    float mFieldOfView;
    int mHeight;
    int mWidth;
    float mAspectRatio;

    QMatrix4x4 mPerspectiveMatrix{};

    float xPosFrus;
    float yPosFrus;
    float zPosFrus;

    Vertex mVerticesFrust[];



};

#endif // CAMERA_H
