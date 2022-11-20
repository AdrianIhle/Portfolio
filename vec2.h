#ifndef VEC2_H
#define VEC2_H

// Small class without cpp-file
#define _USE_MATH_DEFINES
#include <QtDebug>
#include <qmatrix4x4.h>

#include <qgl.h>    // definerer GLfloat
#include <cmath>
#include "vec3.h"

struct Vec2
{
	GLfloat x;
	GLfloat y;
	Vec2(GLfloat a=0.0, GLfloat b=0.0) : x(a), y(b) { }
	const Vec2& operator = (const Vec2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}
	Vec2 operator + (const Vec2& v) const
	{
		Vec2 u;
		u.x = x + v.x;
		u.y = y + v.y;
		return u;
	}
	Vec2 operator - (const Vec2& v) const
	{
		Vec2 u;
		u.x = x - v.x;
		u.y = y - v.y;
		return u;
	}

    Vec3 barysentricCalculation(Vec2 P1, Vec2 P2, Vec2 P3)
    {
        GLfloat u = 0;
        GLfloat v = 0;
        GLfloat w = 0;

        //Making 2 vectors for area calculation
        Vec2 p1p2 = P2 - P1;
        Vec2 p1p3 = P3 - P1;

        //Calculating the area of the entire triangle
        GLfloat triangleArea = p1p2.x * p1p3.y - p1p2.y * p1p3.x;

        //Making 3 vectors for calculating the area of the triangles made by the additional point
        Vec2 pp1 = P1 - Vec2(x, y);
        Vec2 pp2 = P2 - Vec2(x, y);
        Vec2 pp3 = P3 - Vec2(x, y);

        u = (pp2.x * pp3.y - pp2.y * pp3.x) / triangleArea;
        v = (pp3.x * pp1.y - pp3.y * pp1.x) / triangleArea;
        w = (pp1.x * pp2.y - pp1.y * pp2.x) / triangleArea;

        return Vec3 (u, v, w);
    }

};

#endif // VEC2_H
