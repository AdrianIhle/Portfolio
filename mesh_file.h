
#ifndef MESH_FILE_H
#define MESH_FILE_H
#include "gameobject.h"
#include "Mesh_vertexbuffer.h"
#include "mesh.h"
#include "transform.h"
#include "camera.h"
class Mesh_file
{
public:
    Mesh_file();
    void move(Vec3 newPos, GameObject *object);
    static GameObject *readFile(std::string filePath, float xPos, float yPos, float zPos, QString objectName);
    static GameObject* Plane(float xPos = 0, float yPos= 0, float zPos = 0, QString name = "Planemesh");
    static GameObject* Frustrum(Camera* cameraIn, float xPos, float yPos, float zPos, QString name);
    //legg til Cube
    static GameObject *Ball(int n=0, float xPos = 0.0, float yPos = 0.0, float zPos = 0.0, float mHastX = 0.0, QString name =" ");
    static GameObject *AxesGizmo(float xPos = 0, float yPos= 0, float zPos = 0);
    static GameObject *Cube(float xPos = 0, float yPos= 0, float zPos = 0, QString name = "Cubemesh");
private:
    GLuint mNumOfVertices;
    float mRadius;
    int mRekursjoner;
    Vec3 m_position;
    int mIndex = 0;
    Vec3 m_hastighet;
    Vertex * mVertices;
    class Transform *mTransform;


    void lagTriangel(const Vec3& v1, const Vec3& v2, const Vec3& v3);
    void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n);
    void UnitBall();


};

#endif // MESH_FILE_H

