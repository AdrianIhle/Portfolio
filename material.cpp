#include "material.h"
#include "constants.h"

#include <QFile>
#include <QDebug>
#include <QMatrix4x4>

Material::Material(int type) : QOpenGLFunctions(), texture(nullptr)
{
	//set up material type accoring to input type
    switch (type)
    {
	//case 2 and 3 is made the same as UNLIT_COLOR for now
    case 2:
    case 3:
    case UNLIT_COLOR:
		fragmentShaderFileName = new QString(Orf::shaderFilePath + "FlatColorF.glsl");
		vertexShaderFileName = new QString(Orf::shaderFilePath + "FlatColorV.glsl");
        initShader();
        break;
    case UNLIT_TEXTURE:
		fragmentShaderFileName = new QString(Orf::shaderFilePath + "FlatTextureF.glsl");
		vertexShaderFileName = new QString(Orf::shaderFilePath + "FlatTextureV.glsl");
        initShader();
        initTextures();
        break;
    }
}

void Material::initShader()
{
    initializeOpenGLFunctions();
    QString vertexCode;
    QString fragmentCode;
    QFile vertexShaderFile(*vertexShaderFileName);
    QFile fragmentShaderFile(*fragmentShaderFileName);

    if (!vertexShaderFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << *vertexShaderFileName << " could not be opened";
    }

    if (!fragmentShaderFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << *fragmentShaderFileName << " could not be opened";
    }

    QTextStream fileIn1(&vertexShaderFile);
    vertexCode = fileIn1.readAll();
    QTextStream fileIn2(&fragmentShaderFile);
    fragmentCode = fileIn2.readAll();
    mShaderProgram = new ShaderProgram();
    mShaderProgram->initShader(vertexCode, fragmentCode);

    int i = 0;
    glGetProgramiv(mShaderProgram->getShaderId(), GL_ACTIVE_ATTRIBUTES, &i);
    //qDebug() << "shaderprogram" << " number of attributes = " << i;

    mPositionAttribute = mShaderProgram->getAttribLocation("aVertexPosition");
    glEnableVertexAttribArray(static_cast<GLuint>(mPositionAttribute));

    mNormalAttribute = mShaderProgram->getAttribLocation("aNormal");
    glEnableVertexAttribArray(static_cast<GLuint>(mNormalAttribute));

    mTextureAttribute = mShaderProgram->getAttribLocation("aTexcoord");
    glEnableVertexAttribArray(static_cast<GLuint>(mTextureAttribute));

    //qDebug() << "mTextureAttribute " << mTextureAttribute;

    mUniformMVPMatrix = mShaderProgram->getUniformLocation("uMVPMatrix");
}

void Material::initTextures()
{
    // Load image
	texture = new QOpenGLTexture(QImage(Orf::assetFilePath + "directions.png").mirrored(false, true));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Material::useMaterial()
{
    mShaderProgram->useProgram();
    if (texture != nullptr)
        texture->bind();
}

GLint Material::getPositionAttribute()
{
    return mPositionAttribute;
}

GLint Material::getNormalAttribute()
{
    return mNormalAttribute;
}

GLint Material::getTextureAttribute()
{
    return mTextureAttribute;
}

void Material::setMVPMatrix(QMatrix4x4 mvpMatrix)
{
    mShaderProgram->setUniformMatrix(mUniformMVPMatrix, 1, GL_FALSE, mvpMatrix.constData());

}

GLint Material::getMVPMatrix()
{
    return mUniformMVPMatrix;
}

