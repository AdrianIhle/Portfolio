#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QOpenGLFunctions>
#include <QString>

/*
//!
//! \brief The ShaderProgram class
//!
//! This class has functions for creating and compiling a vertex shader and a fragment shader, given
//! the shader source code. The usage of the class is to call initShader() and useProgram() once a
//! ShaderProgram has been created ba the constructor. Another important function setUniformMatrix()
//! which will be called frequently from the class responsible for rendering. Furthermore there are some
//! function for setting and retrieving shaderprogram variables, with the same names as in OpenGL API except
//! for the gl prefix.
//!
//! \author Dag Nylund
//! \date 16/02/08
//!
//!
//!
*/

class ShaderProgram : protected QOpenGLFunctions
{
private:
	GLuint m_shaderProgramObjekt;

    /**
       The shader() function is similar to loadShader() in OpenGL ES3.0 Programming Guide.
       Makes a QString into parameters for glShaderSource
       @param shaderType: GL_VERTEX_SHADER eller GL_FRAGMENT_SHADER
       @param shaderCode: kildekode for aktuell shader.
       @return
     */
    GLuint shader(GLenum shaderType, const QString& shaderCode);

public:
    /// constructor
    ShaderProgram();
    /// destructor calls glDeleteProgram()

    ~ShaderProgram();

    void initShader(const QString& vertexCode, const QString& fragmentCode);

    GLint getAttribLocation(const GLchar* name);
    GLint getUniformLocation(const GLchar* name);
    void setUniformMatrix(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void setUniformVec3(GLint location, float x, float y, float z);
    void useProgram();
    void deleteProgram();
	GLuint getShaderId() { return m_shaderProgramObjekt; }

};

#endif // SHADERPROGRAM_H
