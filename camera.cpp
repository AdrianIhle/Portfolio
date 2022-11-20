#include "camera.h"
#include "vec2.h"
#include "mesh.h"
#include "Mesh_vertexbuffer.h"
#include "Mesh_file.h"
#include "vertex.h"

Camera::Camera(int height, int width, float nearPlane, float farPlane, float fov, QString name)
    : mNearPlane(nearPlane), mFarPlane(farPlane), mFieldOfView(fov), mHeight(height), mWidth(width), mObjectname(name)
{
    mView.setToIdentity();

    //getViewMatrix();
    mForward = Vec3(0,0,-1);
    //mWorldUp = Vec3(1,0,0);
    mUp = Vec3(0,1,0);
    mObjectname = name;
    mAspectRatio = static_cast<float>(mWidth)/static_cast<float>(mHeight ? mHeight : 1);//sets the height and width and their relationship.
    mBackgroundColor = QVector4D(0.3f, 0.3f, 0.3f, 1.0f);
    mPerspectiveMatrix.perspective(mFieldOfView, mAspectRatio, mNearPlane, mFarPlane);

    setFrustrum();
    //mPerspectiveMatrix.frustum(-1.0f, 1.0f, -1.0f, 1.0f, mNearPlane, mFarPlane);
}

void Camera::rotate(float angle, const QVector3D &axis)
{
    mView.rotate(angle, axis);
}

void Camera::translate(float x, float y, float z)
{
    mView.translate(x, y, z);
}

void Camera::setAspectRatio(int w, int h, bool ortho)
{
    mAspectRatio = static_cast<float>(w)/static_cast<float>(h ? h : 1);
    mPerspectiveMatrix.setToIdentity();

    if (ortho)
    {
        float scale{0.01f};
        mPerspectiveMatrix.ortho(-scale*w, scale*w, -scale*h , scale*h, mNearPlane, mFarPlane);
    }
    else
        mPerspectiveMatrix.perspective(mFieldOfView, mAspectRatio, mNearPlane, mFarPlane);
    setFrustrum();
}

void Camera::setBackgroundColor(float r, float g, float b, float a)
{
    mBackgroundColor = QVector4D(r, g, b, a);
}


//QMatrix4x4 Camera::getMatrix()
//{
//    mModelMatrix.setToIdentity();
//    //mModelMatrix.scale(mTransform->getScale().x, mTransform->getScale().y, mTransform->getScale().z);
//    mModelMatrix.rotate(mTransform->getRotation().x, 1.0, 0.0, 0.0);
//    mModelMatrix.rotate(mTransform->getRotation().y, 0.0, 1.0, 0.0);
//    mModelMatrix.rotate(mTransform->getRotation().z, 0.0, 0.0, 1.0);
//    mModelMatrix.translate(-mTransform->getPosition().x, -mTransform->getPosition().y, -mTransform->getPosition().z );
//    //mModelMatrix.translate(mTransform.mPosition);
//    return mModelMatrix;
//}

unsigned int Camera::drawGeometry()
{

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    int offset = 0; //offset for position Positions are the frist data therfor offset is 0


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    mMaterial->useMaterial();
    //getMatrix(); //refresh modelMatrix

    mMVPMatrix = mProjectionMatrix*mViewMatrix*( getMatrix().inverted());
    mMaterial->setMVPMatrix(mMVPMatrix);


    //Tell OpenGL programmable pipeline how to locate vertex position data
    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getPositionAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    // Offset for vertex coordinates
    // before normals
    offset += sizeof(Vec3);
    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getNormalAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));
    // Offset for normal coordinates
    // before UVs
    offset += sizeof(Vec3);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    glVertexAttribPointer(static_cast<GLuint>(mMaterial->getTextureAttribute()), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset));

    glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_SHORT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(0);

    return 0;

}

int Camera::initGeometry()
{
    return 0;
}

void Camera::invertCam()
{
    mProjectionMatrix.inverted();
    mPerspectiveMatrix.inverted();
    mView.inverted();
}

bool Camera::insideFrustum(Vec3 pos, float radius)
{

    GLfloat u,v,w;
    QVector4D v4(pos.x,pos.y,pos.z,1);
    v4 = getMatrix() * v4;
    pos.x = v4.x();
    pos.y = v4.y();
    pos.z = v4.z();

    Vec3 temp = mTransform->getPosition();
    Vec3 camPos = Vec3(temp.x,temp.y,temp.z);

    bary(Vec3(pos.x,0,pos.z), Vec3(0,0,+radius), Vec3(mFrustumFarPoints[0].x-radius,0,mFrustumFarPoints[0].z-radius), Vec3(mFrustumFarPoints[1].x+radius,0,mFrustumFarPoints[1].z-radius), u, v, w, 0);
    if(u>=0 && u<=1 && v>=0 && v<=1 && w>=0 && w<=1)
    {
        bary(Vec3(0,pos.y,pos.z),Vec3(0,0,+radius), Vec3(0,mFrustumFarPoints[0].y-radius,mFrustumFarPoints[0].z-radius), Vec3(0,mFrustumFarPoints[2].y+radius,mFrustumFarPoints[2].z-radius), u, v, w, 1);
        if(u>=0 && u<=1 && v>=0 && v<=1 && w>=0 && w<=1)
        {
            float length = (pos - camPos).length();
            if(length < mNearPlane + radius)
                return false;
            return true;
        }

    }
    return false;
}

QMatrix4x4 Camera::worldToViewMatrix()
{

    QMatrix4x4 mat;
    QVector3D pos(mPosition.x,mPosition.y,mPosition.z);
    QVector3D dir(mForward.x,mForward.y,mForward.z);
    QVector3D up(mUp.x,mUp.y,mUp.z);
    mat.lookAt(pos,pos+dir,up);
    mView = mat;
    return mat;
}

void Camera::bary(Vec3 p, Vec3 v0, Vec3 v1, Vec3 v2, GLfloat &u, GLfloat &v, GLfloat &w, int axes)
{
    if(axes == 0)
    {
        Vec3 p0p1 = v1 - v0;
        Vec3 p0p2 = v2 - v0;

        GLfloat triangleArea = p0p1.x * p0p2.z - p0p1.z * p0p2.x;

        Vec3 pp0 = v0 - p;
        Vec3 pp1 = v1 - p;
        Vec3 pp2 = v2 - p;

        u = (pp1.x * pp2.z - pp1.z * pp2.x) / triangleArea;
        v = (pp2.x * pp0.z - pp2.z * pp0.x) / triangleArea;
        w = (pp0.x * pp1.z - pp0.z * pp1.x) / triangleArea;
    }
    else if(axes == 1)
    {
        Vec3 p0p1 = v1 - v0;
        Vec3 p0p2 = v2 - v0;

        GLfloat triangleArea = p0p1.y * p0p2.z - p0p1.z * p0p2.y;

        Vec3 pp0 = v0 - p;
        Vec3 pp1 = v1 - p;
        Vec3 pp2 = v2 - p;

        u = (pp1.y * pp2.z - pp1.z * pp2.y) / triangleArea;
        v = (pp2.y * pp0.z - pp2.z * pp0.y) / triangleArea;
        w = (pp0.y * pp1.z - pp0.z * pp1.y) / triangleArea;
    }
}

void Camera::setFrustrum()
{

    std::vector<Vertex> vertices;
    Vec3 color(1.0f, 0.0f, 1.0f);
    vertices.push_back(Vertex(Vec3(-1.0f, -1.0f, 1.0f), color, Vec2()));
    vertices.push_back(Vertex(Vec3(1.0f, -1.0f, 1.0f), color, Vec2()));
    vertices.push_back(Vertex(Vec3(1.0f, 1.0f, 1.0f), color, Vec2()));
    vertices.push_back(Vertex(Vec3(-1.0f, 1.0f, 1.0f), color, Vec2()));
    vertices.push_back(Vertex(Vec3(-1.0f, -1.0f, -1.0f), color, Vec2()));
    vertices.push_back(Vertex(Vec3(1.0f, -1.0f, -1.0f), color, Vec2()));
    vertices.push_back(Vertex(Vec3(1.0f, 1.0f, -1.0f), color, Vec2()));
    vertices.push_back(Vertex(Vec3(-1.0f, 1.0f, -1.0f), color, Vec2()));

    QMatrix4x4 inverseP = getPerspectiveMatrix().inverted();

    for (int i{0}; i < 8; ++i)
    {
        QVector4D tempVert(vertices[i].get_x(), vertices[i].get_y(), vertices[i].get_z(), 1.0f);
        QVector4D vertexTransposed = inverseP * tempVert;
        vertexTransposed = vertexTransposed / vertexTransposed.w();
        vertices[i].set_xyz(Vec3(vertexTransposed.x(), vertexTransposed.y(), vertexTransposed.z()));
    }

    mFrustumFarPoints[0] = vertices[0].get_xyz();
    mFrustumFarPoints[1] = vertices[1].get_xyz();
    mFrustumFarPoints[2] = vertices[3].get_xyz();

    GLushort indices[]=
    {
        0,1,2,3
        ,7,6,5,4,0,
        3,7,4,5,
        1,2,6
    };


    initializeOpenGLFunctions();
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(Vertex),&vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 16*sizeof(GLushort),indices, GL_STATIC_DRAW);



}

void Camera::setRendermode(int mode)
{
    if (mode == 1)
        mWireFrame = true;
    else if (mode == 0)
        mWireFrame = false;
}
void Camera::setMaterial(Material *materialIn)
{
    mMaterial = materialIn;
}
