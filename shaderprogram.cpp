#include "shaderprogram.h"

ShaderProgram::ShaderProgram() : QOpenGLFunctions(), m_shaderProgramObjekt(0)
{

}

ShaderProgram::~ShaderProgram()
{

    int status;
    glGetProgramiv(m_shaderProgramObjekt, GL_DELETE_STATUS, &status);
    glDeleteProgram(m_shaderProgramObjekt);
}

GLuint ShaderProgram::shader(GLenum shaderType, const QString& shaderCode)
{
    /// - calls glCreateShader()
    GLuint shader = glCreateShader(shaderType); //GL_VERTEX_SHADER or GL_FRAGMENT_SHADER

    if (shader == 0)
    {
        return 0;
    }

    std::string s = shaderCode.toStdString(); //gets path
    const char* cShaderCode = s.c_str(); //passes the path to a constant char to put in GLShaderSource
    /// - calls glShaderSource() which bind shader to the shadersource/ graphic card
    /// Must happen BEFORE compilation
    glShaderSource(shader, 1, &cShaderCode, 0);
    /// - calls glCompileShader(), which compiles the shader
    glCompileShader(shader); //this makes the shader part of the shader program

    /// - checks compile status
    GLint compiled; //GLUint acts a bool, whether it has compiled or not
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); //acquires the shader ID i think?

    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = new char[infoLen];
            glGetShaderInfoLog(shader, infoLen, 0, infoLog);

            delete [] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }
    /// - get shader source from shader and outputs with qDebug()
    char src[2000]; // Økt fra 500
    int bufLen;
    glGetShaderSource(shader, 2000, &bufLen, src);
    //qDebug() << src;
    return shader;
}

void ShaderProgram::initShader(const QString& vertexCode, const QString &fragmentCode)
{
    //qDebug() << "initShader()";

    initializeOpenGLFunctions(); //initilizes OpenGL to this context

    /// - glCreateProgram()
    m_shaderProgramObjekt = glCreateProgram(); //creates a shader program from input
    //qDebug() << "mShaderProgramObjekt" << mShaderProgramObjekt;

    /// - shader() returns a compiled shader with source which is attached to
    ///   shaderProgram by glAttachShader() for both vertexShader and fragmentShader

    GLuint vertexShader = shader(GL_VERTEX_SHADER, vertexCode); //return the vertex shader Id
    glAttachShader(m_shaderProgramObjekt, vertexShader);        //attaches the vertexshader to the shader program, input is the ID of the program and the id of the shader to bind to it

    GLuint fragmentShader = shader(GL_FRAGMENT_SHADER, fragmentCode); //same for fragment
    glAttachShader(m_shaderProgramObjekt, fragmentShader);

    // if we use glBindAttribLocation, this must happen before the linking.
    // trying to use glGetAttribLocation for now
//    glBindAttribLocation(mShaderProgramObjekt, 0, "aVertexPosition");
//    glBindAttribLocation(mShaderProgramObjekt, 1, "aNormal");
//    glBindAttribLocation(mShaderProgramObjekt, 2, "aTexcoord");


    /// - glLinkProgram() links the vertex and fragment shader together to make a full shader
    glLinkProgram(m_shaderProgramObjekt);

    //dunno what this is
    GLint linked=-1;
    glGetProgramiv(m_shaderProgramObjekt, GL_LINK_STATUS, &linked);

    if (linked==GL_FALSE) //!linked)
    {

        GLint infoLen = 0;
        glGetProgramiv(m_shaderProgramObjekt, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = new char[infoLen];
            glGetShaderInfoLog(m_shaderProgramObjekt, infoLen, 0, infoLog);

            delete [] infoLog;
        }
        glDeleteProgram(m_shaderProgramObjekt);
        m_shaderProgramObjekt = 0;
    }
    /// - now the shaderprogram is ready for use
}

void ShaderProgram::useProgram()
{
    glUseProgram(m_shaderProgramObjekt); //tells GL to use this specific shader, often called just before using shader
}

GLint ShaderProgram::getAttribLocation(const GLchar* name)
{
    GLint i = glGetAttribLocation(m_shaderProgramObjekt, name);
    if(i < 0)
       //qDebug() << name << " not found ...";
    //qDebug() << "shaderprogram "  << "attributt" << i << " for " << name;
    glGetProgramiv(m_shaderProgramObjekt, GL_ACTIVE_ATTRIBUTES, &i);
    //qDebug() << "shaderprogram" << "antall attributter = " << i;

    return glGetAttribLocation(m_shaderProgramObjekt, name);
}

GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
    GLint i = glGetUniformLocation(m_shaderProgramObjekt, name);
    if(i < 0)
     //  qDebug() << name << " not found ...";
    //qDebug() << "shaderprogram" << " uniform" << i << " for " << name;
    glGetProgramiv(m_shaderProgramObjekt, GL_ACTIVE_UNIFORMS, &i);
    //qDebug() << "shaderprogram" << "antall uniforms = " << i;

    return glGetUniformLocation(m_shaderProgramObjekt, name);
}
void ShaderProgram::setUniformMatrix(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    glUniformMatrix4fv(location, count, transpose, value);
}
void ShaderProgram::setUniformVec3(GLint location, float x, float y, float z)
{
    glUniform3f(location, x, y, z);
    /* tested, ok
    float v[3];
    glGetUniformfv(m_shaderProgram, location, v);
    qDebug() << "Uniform vec3 " << v[0] << ", " << v[1] << ", " << v[2];
    */
}



void ShaderProgram::deleteProgram()
{
    GLint i;
    glDeleteProgram(m_shaderProgramObjekt);
    glGetProgramiv(m_shaderProgramObjekt, GL_DELETE_STATUS, &i);
//    if (i == GL_FALSE)
////        qDebug() << "shaderprogram delete status = GL_FALSE";
//    else
////        qDebug() << "shaderprogram delete status = GL_TRUE";
}
