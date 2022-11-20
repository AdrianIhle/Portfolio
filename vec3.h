// Vec3.h - Dag Nylund 15/04/15
// Small class without cpp-file

#ifndef VEC3_H
#define VEC3_H
#define _USE_MATH_DEFINES
#include <QtDebug>
#include <qmatrix4x4.h>

#include <qgl.h>    // definerer GLfloat
#include <cmath>

struct Vec3
{
	GLfloat x;
	GLfloat y;
	GLfloat z;

	Vec3(GLfloat a=0.0, GLfloat b=0.0, GLfloat c=0.0) : x(a), y(b), z(c)
	{
	}

	const Vec3& operator = (const Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	Vec3 operator + (const Vec3& v) const
	{
		Vec3 u;
		u.x = x + v.x;
		u.y = y + v.y;
		u.z = z + v.z;
		return u;
	}
	Vec3 operator - (const Vec3& v) const
	{
		Vec3 u;
		u.x = x - v.x;
		u.y = y - v.y;
		u.z = z - v.z;
		return u;
	}

	// Cross product
	Vec3 operator ^ (const Vec3& v) const
	{
		Vec3 w;
		w.x =  y*v.z - z*v.y;
		w.y =  z*v.x - x*v.z;
		w.z =  x*v.y - y*v.x;
		return w;
	}
	void normalize()
	{
		GLfloat d = x*x+y*y+z*z;
		d = sqrt(d);
		if (d>0.0)
		{
			x=x/d;
			y=y/d;
			z=z/d;
		}
	}

	Vec3 normalized()
	{
		Vec3 normalized;
		GLfloat d = x*x+y*y+z*z;
		d = sqrt(d);
		if (d>0.0)
		{
			normalized.x=x/d;
			normalized.y=y/d;
			normalized.z=z/d;
		}
		return normalized;
	}

	void rotateX(float angle)
	{
		Vec3 dir;
		angle = deg2Rad(angle);
		qDebug() << "Angle: " << angle;
		dir.x = x;
		dir.y = y * cos(angle) + z * -sin(angle);
		dir.z = y * sin(angle) + z * cos(angle) ;

		x = dir.x;
		y = dir.y;
		z = dir.z;
	}

	void rotateY(float angle)
	{
		Vec3 dir;
		angle = deg2Rad(angle);
		qDebug() << "Angle: " << angle;
		dir.x = x * cos(angle) + z * sin(angle);
		dir.y = y;
		dir.z = x * -sin(angle) + z * cos(angle);

		x = dir.x;
		y = dir.y;
		z = dir.z;
	}

	void rotateZ(float angle)
	{
		Vec3 dir;
		angle = deg2Rad(angle);
		qDebug() << "Angle: " << angle;
		dir.x = x * cos(angle) + y * -sin(angle);
		dir.y = x * sin(angle) + y * cos(angle);
		dir.z = z;

		x = dir.x;
		y = dir.y;
		z = dir.z;
	}

	// Dot product
	float operator * (const Vec3& v) const
	{
		return x*v.x + y*v.y + z*v.z;
	}


	Vec3 operator * (float c) const
	{
		return Vec3(x*c, y*c, z*c);
	}

	Vec3 operator * (QMatrix4x4 q) const
	{
//            -------------- MATRIX MULTIPLICATION ----------------------
//            | 0  | 1  | 2  | 3  |
//            | 4  | 5  | 6  | 7  |        X         | x  | y  | z  | 1 |
//            | 8  | 9  | 10 | 11 |
//            | 12 | 13 | 14 | 15 |


			float _x,_y,_z,_w;
		 _x = (q.column(0).x() * x + q.column(0).y() * y + q.column(0).z() * z + q.column(0).w());
		 _y = (q.column(1).x() * x + q.column(1).y() * y + q.column(1).z() * z + q.column(1).w());
		 _z = (q.column(2).x() * x + q.column(2).y() * y + q.column(2).z() * z + q.column(2).w());
		 _w = (q.column(3).x() * x + q.column(3).y() * y + q.column(3).z() * z + q.column(3).w());

		 if(_w == 1.0f)
			 return Vec3(_x, _y, _z);
		 else
			 return Vec3(_x/_w, _y/_w, _z/_w);
	}

	float length()
	{
		return sqrt(x*x+y*y+z*z);
	}

	float rad2Deg(float &rad)
	{
        return (rad * 180)/3.1418f;
	}

	float deg2Rad(float &deg)
	{
        return (deg * 3.1418f)/180;
	}



	//Operator overloading som friendfunksjon
//	   friend std::ostream &operator<<(std::ostream &output, const Vec3 &vIn )
//	   {
//		   //output << "X = ": // << vIn.x << ", Y = " << vIn.y << ", Z = " << vIn.z; // << std::endl;
//		   return output;
//	   }
};
#endif // VEC3_H
