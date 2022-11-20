#ifndef PLANE_H
#define PLANE_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include "transact.h"
#include "transform.h"
#include "gameobject.h"
#include "vec3.h"
#include "vec2.h"
#include "vertex.h"

class Plane : public GameObject
{
public:
    Plane(float xPos = 0, float yPos = 0, float zPos = 0, int mSize = 5, float mSpacing = 5, QString name = "Plane", QString type = "Plane");
    void setRendermode(int mode);
    void setMaterial( Material *MaterialIn);
    unsigned int drawGeometry();
    int mapSize = 20; //Mapsize has to be set to same pixel as heightmap, sets how many verticies there will be//changed mapsize to 20

    Vec3 findTriangle(Vec2 objectPos);          //searches through the plane for the triangle the object is on by checking for its barycentric coordinates and which triangle they are placed within
    void colorTriangle(Vec2 objectPos);         //colors triangle at the position given
    Vec3 normalOfCurrentTriangle();             //used to find the normal of the triangle the object is on, for collision calculation
    float angleOfCurrentTriangle();             //returns the angle of the current triangel in relation to a flat laying triangle, to determine which way an object would roll
    Vertex *vertices;
    GLuint getVertexBuffer () {return mVertexBuffer;};
    void refreshVertices();
    int initGeometry();
private:



    float mapFactor = 3.0; //scales width/length between verticies. Change the size
    GLsizei indexCount;
    QMatrix4x4 mModelMatrix;
    GLuint mVertexBuffer;
    GLuint mIndexBuffer;
    Material * mMaterial;
    QMatrix4x4 mMVPMatrix;

    Vec3 previousTriangle = Vec3 (0,0,0);  //stores the position of the previous triangle the object was on
    Vec3 prevColorP1 = 0;             //stores the colors of the previous triangle
    Vec3 prevColorP2 = 0;
    Vec3 prevColorP3 = 0;

    float mapSpacing;

};

#endif // PLANE_H
