#include "SOSIplane.h"
#include "vec3.h"

#include <QtDebug>

SOSIplane::SOSIplane(QString path, float scale, float heightDivide, float xPos, float yPos, float zPos, QString nameIn) : GameObject(xPos, yPos, zPos, nameIn)
{
    mScale = scale;
    mHeightScale = heightDivide;
    initGeometry(path.toStdString());
}

int SOSIplane::initGeometry()
{
    // Transfer vertex data to VBO 0
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numOfVertices*sizeof(Vertex), mVertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices*sizeof(GLushort), mIndices, GL_STATIC_DRAW);

    return 0;
}

int SOSIplane::initGeometry(std::string filePath)
{
    const char* path = filePath.c_str();

    //Open File
    FILE * file = fopen(path, "r");

    if(file == nullptr)
    {
        qDebug() << "Could not open file at: " << path;
    }

    char oneWord[128] = "";
    int remaining;
    bool first = true;
    Vec3 removeOffset;

    while(true)
    {
        remaining = fscanf(file, "%s", &oneWord);

        std::vector<Vec3> tempPos;
        std::vector<int> tempIndices;
        float currentHeight{0};
        float currentIndex{0};

        if(remaining == EOF)
        {
            break;
        }

        if(strcmp(oneWord, ".KURVE") == 0)
        {
            while(true)
            {
                remaining = fscanf(file, "%s", &oneWord);

                if(strcmp(oneWord, "..HØYDE") == 0)
                {
                    remaining = fscanf(file, "%s", &oneWord);
                    currentHeight = std::stof(oneWord);
                }
                else if (strcmp(oneWord, "..NØ") == 0)
                {
                    while(true)
                    {
                        Vec3 tempVec(0.0f, currentHeight, 0.0f);
                        remaining = fscanf(file, "%s", &oneWord);

                        if (strcmp(oneWord, "...KP") == 0)
                        {
                            remaining = fscanf(file, "%s", &oneWord);
                            remaining = fscanf(file, "%s", &oneWord);
                        }

                        if ((strcmp(oneWord, ".KURVE") == 0)||(strcmp(oneWord, ".SLUTT") == 0))
                        {
                            tempIndices.pop_back();
                            tempIndices.pop_back();
                            break;
                        }

                        if (strcmp(oneWord, "..NØ") == 0)
                            remaining = fscanf(file, "%s", &oneWord);

                        tempVec.z = std::stof(oneWord);
                        remaining = fscanf(file, "%s", &oneWord);
                        tempVec.x = std::stof(oneWord);

                        if (first)
                        {
                            removeOffset = tempVec;
                            removeOffset.y = 0;
                            first = false;
                        }

                        tempVec = (tempVec-removeOffset);
                        tempVec.x = tempVec.x * mScale;
                        tempVec.z = tempVec.z * mScale;
                        tempPos.push_back(tempVec);

                        tempIndices.push_back(currentIndex);
                        currentIndex++;
                        tempIndices.push_back(currentIndex);
                        //qDebug() << tempVec.x << " " << tempVec.y << " " << tempVec.z << " " << tempPos.size();
                    }
                }

                if (strcmp(oneWord, ".SLUTT") == 0)
                    break;
            }

            numOfVertices = tempPos.size();
            numOfIndices = tempIndices.size();

            mVertices = new Vertex[numOfVertices];
            mIndices = new GLushort[numOfIndices];

            for (int i = 0; i < numOfVertices; i++)
            {
                mVertices[i].set_xyz(tempPos[i]);
                if (int(mVertices[i].get_xyz().y) % 100 == 0)
                {
                    mVertices[i].set_rgb(1.0f, 1.0f, 0.0f);
                    qDebug() << mVertices[i].get_xyz().y;
                }
                else
                    mVertices[i].set_rgb(1.0f, 0.0f, 0.0f);

                mVertices[i].set_xyz(mVertices[i].get_xyz().x, mVertices[i].get_xyz().y * mHeightScale, mVertices[i].get_xyz().z);
            }

            for (int i = 0; i < numOfIndices; i++)
            {
                mIndices[i] = tempIndices[i];
            }
        }
    }

    fclose(file);

    initializeOpenGLFunctions();

    initGeometry();
}

unsigned int SOSIplane::drawGeometry()
{
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    mMaterial->useMaterial();

    //refresh modelMatrix:
    getMatrix();

    mMVPMatrix = mProjectionMatrix*mViewMatrix*mModelMatrix;
    mMaterial->setMVPMatrix(mMVPMatrix);

    // Offset for position
    // Positions are the first data, therefor offset is 0
    int offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
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

    // Draw cube geometry using indices from VBO 1
     glDrawElements(GL_LINES, numOfIndices, GL_UNSIGNED_SHORT, 0);

    //Unbind buffers:
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Unbind shader
    glUseProgram(0);
    return numOfIndices;
}

void SOSIplane::setRendermode(int mode)
{
    if (mode == 1)
        mWireFrame = true;
    else if (mode == 0)
        mWireFrame = false;
}

void SOSIplane::setMaterial(Material *materialIn)
{
    mMaterial = materialIn;
}
