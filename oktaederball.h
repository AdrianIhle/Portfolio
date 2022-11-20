#ifndef OKTAEDERBALL_H
#define OKTAEDERBALL_H
#include <cmath>
#include <string>
#include "gameobject.h"
#include "vec3.h"
#include "material.h"
#include "transform.h"


class Vertex;
class OktaederBall : public GameObject
{
private:
    int mRecursions = 0;
    int mIndex = 0;
    float mRadius = 0;

    Vertex* mVertices;

    GLuint mNumOfVertices;

    void lagTriangel(const Vec3& v1, const Vec3& v2, const Vec3& v3);
    void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n);
    void oktaederUnitBall();
public:
    OktaederBall(float xPos = 0, float yPos = 0 , float zPos = 0, int n=0, QString name = "Ball", QString type = "Ball");
    ~OktaederBall();
    int initGeometry();
    unsigned int drawGeometry();
    void setRendermode(int mode);
    void move(const Vec3 &newVelocity, float dt = 0.017f);

    void setMaterial(Material *materialIn);



};

#endif
