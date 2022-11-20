#include <iostream>
#include "barycetrictriangulation.h"

using namespace std;

BarycetricTriangulation::BarycetricTriangulation(Plane* n)
{
    nPlane = n;

}


bool BarycetricTriangulation::BarycentricCoordinates(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p0)
{

    Vec3 p1p2 = p1 - p2;
    Vec3 p2p3 = p2 - p3;
    Vec3 p3p1 = p3 - p1;

    Vec3 p1p0 = p1 - p0;
    Vec3 p2p0 = p2 - p0;
    Vec3 p3p0 = p3 - p0;

    float t = (p1p2 ^ p2p3).z;
    float u = (p1p0 ^ p1p2).z / t;
    float v = (p2p0 ^ p2p3).z / t;
    float w = (p3p0 ^ p3p1).z / t;

    float barycentricValue = u + v + w;

    qDebug() << "u: " << u << endl;
    qDebug() << "v: " << v << endl;
    qDebug() << "w: " << w << endl;

    qDebug() << "barycentric value: " << barycentricValue << endl;


    if (u >= 0.0f && v >= 0.0f && w >= 0.0f)
    {
        if(barycentricValue == 1)
            return true;
        else
            return false;
    }

    return false;
}


/*
void BarycetricTriangulation::Triangulation( Vec3 P0)
{
    int trigNrOfVertices = 0;
    Vec3 u;
    Vec3 v;
    Vec3 w;

    Vertex r;
    Vertex s;
    Vertex t;

    for (int i = 0; i < nPlane->mapSize*nPlane->mapSize; i++) {

        trigNrOfVertices++;

        if(trigNrOfVertices == 1)
        {
            u = nPlane->mVertArray[i].get_xyz();
            //            r = nPlane->mVertArray[i];
            qDebug()<< "u0: "<<  u.x << "," << u.y << ","<< u.z << endl;
        }
        else if(trigNrOfVertices == 2)
        {
            v = nPlane->mVertArray[i].get_xyz();
            //            s = nPlane->mVertArray[i];
            qDebug()<< "v0: "<<  v.x << "," <<v.y << ","<< v.z << endl;
        }
        else if(trigNrOfVertices == 3)
        {
            w = nPlane->mVertArray[i].get_xyz();
            //            t = nPlane->mVertArray[i];
            qDebug()<< "w0: "<<  w.x << "," << w.y << ","<< w.z << endl;

            if(BarycentricCoordinates(u,v,w,P0) == true)
                goto endOfLoop;
            trigNrOfVertices = 0;
        }
    }

endOfLoop:
    if(BarycentricCoordinates(u,v,w,P0) == true)
    {

        qDebug()<< "Barycentric is true"<<endl;
        //change color
        r.set_rgb(1,1,1);
        s.set_rgb(1,1,1);
        t.set_rgb(1,1,1);
    }
    else
        std::cout << "ball not found" << endl;


}
*/

/*this method is based on the fact that the map is setup up in rows and columns. this way it searches through the plane row by row, testing if the object is in said triangle
 * if not the second if statement reassigns the temp int to reflect the next triangle (by traversing the the plane by row down and addnt the relation of the other vertices
 * with exception given in the else statement where special reassignment is given manually
 */
Vec3 BarycetricTriangulation::BarycentricLoop( Vec2 *objPos)
{
    // Start at row 1. Set vertices for first triangle. These will always be the same
    int row = 1;
    int tempP1 = 1;
    int tempP2 = 0;
    int tempP3 = nPlane->mapSize;

    for(int triangle = 0; triangle < (nPlane->mapSize-1)*(nPlane->mapSize-1)*2; triangle++)
    {
        //Calculating barycentric coordinates
        Vec3 baryCoor = objPos->barysentricCalculation(nPlane->vertices[tempP1].get_xz(), nPlane->vertices[tempP2].get_xz(), nPlane->vertices[tempP3].get_xz());

        //Checking if the barycentric coordinates confirm that the object is within the triangle
        if(baryCoor.x > 0 && baryCoor.x < 1 && baryCoor.y > 0 && baryCoor.y < 1 && baryCoor.z > 0 && baryCoor.z < 1)
        {
            //Return a vec3 holding the indices of the 3 vertices making a triangle
            return Vec3 (tempP1, tempP2, tempP3);
        }

        //If the object is not within the triangle, set values for next triangle
        if (tempP3 == ((row + 1)*nPlane->mapSize - 1))
        {
            tempP1 = row * nPlane->mapSize + 1;
            tempP2 = row * nPlane->mapSize;
            tempP3 = (row + 1)*nPlane->mapSize;
            row++;
        }
        else
        {
            if (triangle % 2 == 0)
            {
                tempP2 += nPlane->mapSize;
                tempP3 ++;
            }
            else if (triangle % 2 == 1)
            {
                tempP1++;
                tempP2 -= (nPlane->mapSize-1);
            }
        }

        //If the object is outside the plane
        if (triangle == nPlane->mapSize*nPlane->mapSize*2)
        {
            qDebug() << "Object is not within the plane";
        }
    }

    return 0;
}

void BarycetricTriangulation::HighlightTriangle( Vec2* Pos)
{
    Vec3 currentTriangle = BarycentricLoop(Pos);                    //finds the current triangle the object is on

    if(previousTriangle.x < 0.000001f)                              //if this is the first time the first triangle in the list then this special condition catches it and stores the previous colors and altes to new
    {
        previousTriangle = currentTriangle;
        prevColorP1 = nPlane->vertices[int(currentTriangle.x)].get_rgb();
        prevColorP2 = nPlane->vertices[int(currentTriangle.y)].get_rgb();
        prevColorP3 = nPlane->vertices[int(currentTriangle.z)].get_rgb();

        nPlane->vertices[int(currentTriangle.x)].set_rgb(1.0f, 1.0f, 1.0f);
        nPlane->vertices[int(currentTriangle.y)].set_rgb(1.0f, 1.0f, 1.0f);
        nPlane->vertices[int(currentTriangle.z)].set_rgb(1.0f, 1.0f, 1.0f);
        nPlane->refreshVertices(); // Send new vertex data to buffer

    }
    else if (currentTriangle.x != previousTriangle.x || currentTriangle.y != previousTriangle.y || currentTriangle.z != previousTriangle.z) //same as above but for remaining triangles
    {
        qDebug() << "reverting color" <<endl;
        //If object is entering new triangle, reset color of old triangle, save color of new triangle, and color new triangle red
        nPlane->vertices[int(previousTriangle.x)].set_rgb(prevColorP1.x, prevColorP1.y, prevColorP1.z);
        nPlane->vertices[int(previousTriangle.y)].set_rgb(prevColorP2.x, prevColorP2.y, prevColorP2.z);
        nPlane->vertices[int(previousTriangle.z)].set_rgb(prevColorP3.x, prevColorP3.y, prevColorP3.z);

        prevColorP1 = nPlane->vertices[int(currentTriangle.x)].get_rgb();
        prevColorP2 = nPlane->vertices[int(currentTriangle.y)].get_rgb();
        prevColorP3 = nPlane->vertices[int(currentTriangle.z)].get_rgb();

        nPlane->vertices[int(currentTriangle.x)].set_rgb(1.0f, 0.0f, 0.0f);
        nPlane->vertices[int(currentTriangle.y)].set_rgb(1.0f, 0.0f, 0.0f);
        nPlane->vertices[int(currentTriangle.z)].set_rgb(1.0f, 0.0f, 0.0f);

        nPlane->refreshVertices();//Send new vertex data to buffer
        previousTriangle = currentTriangle;



    }
}







