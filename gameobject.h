#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "vertex.h"
#include "material.h"
#include "vec3.h"
#include "component.h"
#include "transform.h"

class Component;


class GameObject : protected QOpenGLFunctions
{
private:

public:
    GameObject(float xPos = 0.0, float yPos = 0.0, float zPos = 0.0, QString nameIn = "GameObject", QString type = "GameObject");
    virtual ~GameObject();


    float xP;
    float yP;
    float zP;


    virtual unsigned int drawGeometry() = 0;
    virtual void setMaterial(Material *materialIn) = 0;
    virtual void setRendermode(int mode) = 0;

    void setViewMatrix(QMatrix4x4 viewMatrix);
    void setprojectionMatrix(QMatrix4x4 projectionMatrix);
    void setName(QString nameIn);

    std::vector<Component*> mComponents;

    void addComponent(Component* inComponent);

    virtual void move() {}

    QString getName();
    QString mType;

    QMatrix4x4 getMatrix();

    class Transform *mTransform;

    GameObject* getParent() { return mParent; }
    void setParent(GameObject *parent);
    Vertex* mVertices;         // minneallokering i avledede klasser
    int initGeometry(Material *m);
    std::vector<GameObject*> mChildrenList;
    float mRadius = 1.0f;


    void updateChildren();
    void updateFromParent();
    void initChildren();
    void drawChildren();
    float getRadius() const;

    QString getType() const;

    void setType(const QString &type);

protected:



    GLuint mAntallVertices;
    GLuint mVertexBuffer;      //GLuint m_normalBuffer; //GLuint m_textureBuffer;
    GLuint mIndexBuffer;

    Vertex* n_vertices;         // minneallokering i avledede klasser
    GLuint n_antallVertices;
    GLuint n_vertexBuffer;      //GLuint m_normalBuffer; //GLuint m_textureBuffer;
    GLuint n_indexBuffer;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

    GameObject *mParent;
    QString name;

    Material *mMaterial;

    QMatrix4x4 mMVPMatrix;

    bool mWireFrame = false;

};

#endif // GAMEOBJECT_H
