#ifndef BALL_H
#define BALL_H

#include <cmath>
#include <string>
#include "gameobject.h"
#include "vec3.h"
#include "material.h"

class Vertex;

//!
//! \brief The OktaederBall class implements recursive subdivision of an Octahedron
//!
//! The class implements recursive subdivision of an regular Octahedron to visualize a ball with
//! glDrawArrays() and GL_TRIANGLES
//!
//! The original Octahedron has 6 vertices on the unit ball:
//! - top (0, 0, 1)
//! - bottom (0, 0, -1)
//! - four equidistant vertices in the xy-plane
//! This 6 vertices defines 8 triangles
//!

//implement drawGeomertry, setMAterial, setRendermode
class ball : public GameObject
{
private:
    int m_rekursjoner;          //
    int m_indeks;               // brukes i rekursjon, til å bygge m_vertices

    void lagTriangel(const Vec3& v1, const Vec3& v2, const Vec3& v3);
    void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n);
    void oktaederUnitBall();

    QMatrix4x4 mMVPMatrix;
    GLuint mVertexBuffer;
    GLuint mIndexBuffer;
    Material *mMaterial;
public:
    ball(float xPs, float yPs, float zPs, int n, QString name);

    ~ball();
    void initVertexBufferObjects();

    float x = xP;
    float y = yP;
    float z = zP;


    int initGeometry();
    void drawGeometry();
    void setMaterial(Material *materialIn);
    void setRendermode(int mode);
};


#endif // BALL_H
