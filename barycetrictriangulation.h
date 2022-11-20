#ifndef BARYCETRICTRIANGULATION_H
#define BARYCETRICTRIANGULATION_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "vec3.h"
#include "plane.h"
#include "gameobject.h"

class BarycetricTriangulation : protected QOpenGLFunctions
{
public:
    BarycetricTriangulation(Plane *n);                              //Takes in the plsne its functions will work on (
    Vec3* BarVec;                                                   //stores the vecotr of the barycentric coordinates
    Plane* nPlane;                                                  //stores the plane being worked on
    bool BarycentricCoordinates(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p0);//calculates the coordinates based on the barycentric formula
//    void Triangulation(Vec3 P0);              //old function kept for reference
    Vec3 BarycentricLoop(Vec2 *objPos);                             //loops through a plane to search for object
    void HighlightTriangle(Vec2 *Pos);                              //highlights the triangle the object is on, calls BarycentricLoop
    void refreshVertexBuffer();                                     //update vertex data to reflect changes made in function (color only so far)

    Vec3 previousTriangle = Vec3 (0,0,0);  //stores the position of the previous triangle the object was on
    Vec3 prevColorP1 = 0;             //stores the colors of the previous triangle
    Vec3 prevColorP2 = 0;
    Vec3 prevColorP3 = 0;


};

#endif // BARYCETRICTRIANGULATION_H
