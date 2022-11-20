#include <QDebug>
#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(Vec3 a, Vec3 b, Vec2 c)
{
	mXYZ = a;
	mNormal = b;
	mST = c;
}

Vertex::~Vertex()
{
	//qDebug() << "Vertex::~Vertex()";
}

void Vertex::set_xyz(GLfloat* xyz)
{
	mXYZ.x = xyz[0];
	mXYZ.y = xyz[1];
	mXYZ.z = xyz[2];
}

void Vertex::set_xyz(GLfloat x, GLfloat y, GLfloat z)
{
	mXYZ.x = x; mXYZ.y = y; mXYZ.z = z;
}

void Vertex::set_xyz(Vec3 xyz_in)
{
	mXYZ = xyz_in;
}

void Vertex::set_rgb(GLfloat* rgb)
{
	mNormal.x = rgb[0];
	mNormal.y = rgb[1];
	mNormal.z = rgb[2];
}

void Vertex::set_rgb(GLfloat r, GLfloat g, GLfloat b)
{
	mNormal.x = r; mNormal.y = g; mNormal.z = b;
}

void Vertex::set_normal(GLfloat* normal)
{
	mNormal.x = normal[0];
	mNormal.y = normal[1];
	mNormal.z = normal[2];
}

void Vertex::set_normal(GLfloat x, GLfloat y, GLfloat z)
{
	mNormal.x = x; mNormal.y = y ; mNormal.z = z;
}

void Vertex::set_normal(Vec3 normal_in)
{
	mNormal = normal_in;
}

void Vertex::set_st(GLfloat* st)
{
	mST.x = st[0];
	mST.y = st[1];
}

void Vertex::set_st(GLfloat s, GLfloat t)
{
	mST.x = s; mST.y = t;
}

void Vertex::set_uv(GLfloat s, GLfloat t)
{
	mST.x = s; mST.y = t;
}

Vec3 Vertex::get_xyz()
{

	return mXYZ;
}


GLfloat Vertex::get_x()
{
	return mXYZ.x;
}

GLfloat Vertex::get_y()
{
	return mXYZ.y;
}

GLfloat Vertex::get_z()
{
	return mXYZ.z;
}

Vec2 Vertex::get_xz()
{
    return Vec2(get_x(), get_z());
}

Vec3 Vertex::get_rgb()
{
    return mNormal;
}

std::ostream& operator<< (std::ostream& os, const Vertex& v)
{
   os << "(" << v.mXYZ.x << ", " << v.mXYZ.y << ", " << v.mXYZ.z << ") ";
   os << "(" << v.mNormal.x << ", " << v.mNormal.y << ", " << v.mNormal.z << ") ";
   os << "(" << v.mST.x << ", " << v.mST.y << ") ";
   return os;
}
std::istream& operator>> (std::istream& is, Vertex& v)
{
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
   char dum, dum2, dum3, dum4;
   is >> dum >> v.mXYZ.x >> dum2 >> v.mXYZ.y >> dum3 >> v.mXYZ.z >> dum4;
   is >> dum >> v.mNormal.x >> dum2 >> v.mNormal.y >> dum3 >> v.mNormal.z >> dum4;
   is >> dum >> v.mST.x >> dum2 >> v.mST.y >> dum3;

   return is;
}
