 #ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include "shaderprogram.h"

class Material : protected QOpenGLFunctions
{
public:
    Material(int type);

    void initShader();
    void initTextures();
    void useMaterial();

    GLint getPositionAttribute();
    GLint getNormalAttribute();
    GLint getTextureAttribute();

    void setMVPMatrix(QMatrix4x4 mvpMatrix);

    GLint getMVPMatrix();

private:

    enum Types
    {
        UNLIT_COLOR,
        UNLIT_TEXTURE,
        LIT_COLOR,
        LIT_TEXTURE
    };

    //The start of compiling the shaders ourselvs:
    //ShaderProgram *mShaderLight;
    ShaderProgram *mShaderProgram;

    QString* fragmentShaderFileName;
    QString* vertexShaderFileName;

    QOpenGLTexture *texture;

	GLint mPositionAttribute;
	GLint mNormalAttribute;
	GLint mTextureAttribute;

	GLint mUniformMVPMatrix;

    //GLint mUniformMVMatrix;
    //GLint mUniformPMatrix;
};

#endif // MATERIAL_H
