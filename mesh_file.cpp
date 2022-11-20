#include "Mesh_file.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include "vec2.h"
#include "vec3.h"
#include "constants.h"
#include <QString>
#include "transform.h"
#include "gameobject.h"
#include "camera.h"


Mesh_file::Mesh_file()
{

}

GameObject* Mesh_file::readFile(std::string fileName, float xPos, float yPos, float zPos, QString objectName)
{
    Mesh *mMesh = new Mesh(xPos, yPos, zPos, objectName);
    MeshBase *mMeshbase = new MeshBase();

    std::vector<Vertex> verticesVector;
    std::vector<GLushort> indicesVector;

    unsigned int vertexSize = 0;
    unsigned int indexSize = 0;
    //Making general arrays for the vertecies and indices
    //    Vertex vertices[vertexSize];
    //    GLushort indices[indexSize];


    // Varible for constructing the indices vector
    int temp_index = 0;

    int lodLevel{0};
    //Open File
    std::string file = Orf::assetFilePath.toStdString() + fileName + ".obj";
    std::ifstream fileIn(file);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(file);

    do
    {
        //One line at a time-variable
        std::string oneLine;
        //One word at a time-variable
        std::string oneWord;

        std::vector<Vec3> tempVertecies;
        std::vector<Vec3> tempNormals;
        std::vector<Vec2> tempUVs;

        //Reading one line at a time from file to oneLine
        while(std::getline(fileIn, oneLine))
        {
            //Doing a trick to get one word at a time
            std::stringstream sStream;
            //Pushing line into stream
            sStream << oneLine;
            //Streaming one word out of line
            sStream >> oneWord;

            if (oneWord == "#")
            {
                //Ignore this line
                //qDebug() << "Line is comment "  << QString::fromStdString(oneWord) << endl;
                continue;
            }
            if (oneWord == "v")
            {
                //qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
                Vec3 tempVertex;
                sStream >> oneWord;
                //stof ignores the white space between words
                tempVertex.x = std::stof(oneWord);
                sStream >> oneWord;
                tempVertex.y = std::stof(oneWord);
                sStream >> oneWord;
                tempVertex.z = std::stof(oneWord);

                //Vertex made - pushing it into vertex-vector
                tempVertecies.push_back(tempVertex);

                continue;
            }
            if (oneWord == "vt")
            {
                //qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
                Vec2 tempUV;
                sStream >> oneWord;
                tempUV.x = std::stof(oneWord);
                sStream >> oneWord;
                tempUV.y = std::stof(oneWord);

                //UV made - pushing it into UV-vector
                tempUVs.push_back(tempUV);

                continue;
            }
            if (oneWord == "vn")
            {
                //qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
                Vec3 tempNormal;
                sStream >> oneWord;
                tempNormal.x = std::stof(oneWord);
                sStream >> oneWord;
                tempNormal.y = std::stof(oneWord);
                sStream >> oneWord;
                tempNormal.z = std::stof(oneWord);

                //Vertex made - pushing it into vertex-vector
                tempNormals.push_back(tempNormal);
                continue;
            }
            if (oneWord == "f")
            {
                //qDebug() << "Line is a face "  << QString::fromStdString(oneWord) << " ";
                //int slash; //used to get the / from the v/t/n - format
                int index, normal, uv;
                for(int i = 0; i < 3; i++)
                {
                    sStream >> oneWord;     //one word read
                    std::stringstream tempWord(oneWord);    //to use getline on this one word
                    std::string segment;    //the numbers in the f-line
                    std::vector<std::string> segmentArray;  //temp array of the numbers
                    while(std::getline(tempWord, segment, '/')) //splitting word in segments
                    {
                        segmentArray.push_back(segment);
                    }
                    index = std::stoi(segmentArray[0]);     //first is vertex
                    if (segmentArray[1] != "")              //second is uv
                        uv = std::stoi(segmentArray[1]);
                    else
                    {
                        //qDebug() << "No uvs in mesh";       //uv not present
                        uv = 0;                             //this will become -1 in a couple of lines
                    }
                    normal = std::stoi(segmentArray[2]);    //third is normal

                    //Fixing the indexes
                    //because obj f-lines starts with 1, not 0
                    --index;
                    --uv;
                    --normal;

                    if (uv > -1)    //uv present!
                    {
                        Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
                        verticesVector.push_back(tempVert);
                    }
                    else            //no uv in mesh data, use 0, 0 as uv
                    {
                        Vertex tempVert(tempVertecies[index], tempNormals[normal], Vec2(0.0f, 0.0f));
                        verticesVector.push_back(tempVert);
                    }

                    indicesVector.push_back(temp_index++);

                }
                continue;
            }

            //if we get down here, the line is something we don't care about.
            //qDebug() << "Line was something strange! " << QString::fromStdString(oneWord);
        }
        //beeing a nice boy and closing the file after use
        fileIn.close();

        vertexSize = verticesVector.size();
        indexSize = indicesVector.size();

        Vertex vertices[vertexSize];
        GLushort indices[indexSize];

        for (int counter{0}; counter < vertexSize; ++counter)
        {
            vertices[counter] = verticesVector[counter];
        }

        for (int counter{0}; counter < indexSize; ++counter)
        {
            indices[counter] = indicesVector[counter];
        }

        //We now have the vertex and index buffer
        //Now we make a meshbase with boundingVolume of it!
        //will still send in suzanne.obj lodlevel 0 here.
        mMeshbase->initGeometry(vertices, vertexSize, indices, indexSize, lodLevel); //lodlevel supposed to be last parameter here

        if(lodLevel == 0)
        {
            BoundingVolume *tempBoundingVolume = new BoundingVolume();
            tempBoundingVolume->initGeometry(vertices, vertexSize);
            mMeshbase->setBoundingVolume(tempBoundingVolume);
        }
        lodLevel++;
        //stops here!! in this if!! Goes to the size function in GL
        //if(objectName == "Mankey" && lodLevel != 0)
        file = Orf::assetFilePath.toStdString() + fileName + "_L0" + std::to_string(lodLevel)  + ".obj";
        fileIn.open (file, std::ifstream::in);
        if(!fileIn)
        {
            qDebug() << "Could not open file for reading: " << QString::fromStdString(file);
            break;      // break out of loop
        }

        //lodLevel++;//goes over to next lod so it's all ready

        //tegnes her for å unngå crash. Crasher hvis int vertexSize etc blir deklarert høyere opp, selv om vi
        //redeklarer eller bare setter nye verdier på den.
        //Tegner bare lodlevel0 det er sannsynligvis pga av at vi initGeometry her.
        //så må få den andre måten å fungere.

    }while(lodLevel < Orf::lodLevels);  //if lod level files found - run once more!
    //find all three levels
    //end of do while

    //Then we make a mesh and assign the meshbase to it
    mMesh->frustrumDraw = false;
    //in the case of objects they don't need this as they get their vertices and indices within the draw function
    //only for boundingvolume and knowing their belonging vertexbuffer
    mMesh->initGeometry(mMeshbase);

    return mMesh;
}

GameObject* Mesh_file::Plane(float xPos, float yPos, float zPos, QString name)
{
    int mapSize = 10;
    GLuint mNumOfVertices= 6 * mapSize * mapSize;
    Vertex *mVertices = new Vertex[mNumOfVertices];

    int MapFactor = 10;
    Vec3 dir(0.0f, 0.0f, 1.0f);
    //  Vec2 tx = Vec2(0.66f, 1.0f);
    float x;
    float y;
    float z;
    int k = 0;
    for(int row = 0; row < (mapSize - 2); row++)
    {
        for(int col = 0; col <  (mapSize - 2) ; col++)
        {
            x = static_cast<float>(row) * MapFactor/mapSize;
            z = static_cast<float>(col)* MapFactor/mapSize;
            y = 0.0f;
            Vec3 pos(x, y, z);
            mVertices[k] = Vertex(pos, dir, Vec2(0.66f, 1.0f));
            k++;
            x = static_cast<float>(row + 1)* MapFactor/mapSize;
            z = static_cast<float>(col)* MapFactor/mapSize;
            y = 0.0f;
            pos = Vec3(x, y, z);
            mVertices[k] = Vertex(pos, dir, Vec2(0.66f, 1.0f));
            k++;
            x = static_cast<float>(row)* MapFactor/mapSize;
            z = static_cast<float>(col + 1)* MapFactor/mapSize;
            y = 0.0f;
            pos = Vec3(x, y, z);
            mVertices[k] = Vertex(pos, dir, Vec2(0.66f, 1.0f));
            k++;

            x = static_cast<float>(row+1)* MapFactor/mapSize;
            z = static_cast<float>(col)* MapFactor/mapSize;
            y = 0.0f;
            pos = Vec3(x, y, z);
            mVertices[k] = Vertex(pos, dir, Vec2(0.66f, 1.0f));
            k++;
            x = static_cast<float>(row + 1)* MapFactor/mapSize;
            z = static_cast<float>(col + 1)* MapFactor/mapSize;
            y = 0.0f;
            pos = Vec3(x, y, z);
            mVertices[k] = Vertex(pos, dir, Vec2(0.66f, 1.0f));
            k++;
            x = static_cast<float>(row)* MapFactor/mapSize;
            z = static_cast<float>(col+1)* MapFactor/mapSize;
            y = 0.0f;
            pos = Vec3(x, y, z);
            mVertices[k] = Vertex(pos, dir, Vec2(0.66f, 1.0f));
            k++;
            //qDebug()<<k;
            //mNumOfVertices = k;
        }

        // mPlane->mTransform->setScale(10, 10, 10);
        //  mPlane->mTransform->setRotation(180, 0, 0);
    }
    MeshBase *mMeshbase = new MeshBase();
    mMeshbase->initGeometry(mVertices, mNumOfVertices, 0, 0);

    BoundingVolume *tempBoundingVolume = new BoundingVolume();
    tempBoundingVolume->initGeometry(mVertices, mNumOfVertices);
    mMeshbase->setBoundingVolume(tempBoundingVolume);

    Mesh *mMesh = new Mesh(xPos, yPos, zPos, name);
    mMesh->frustrumDraw = false;
    mMesh->initGeometry(mMeshbase);

    return mMesh;
}

GameObject* Mesh_file::Frustrum(Camera* cameraIn, float xPos, float yPos, float zPos, QString name)
{
    int lodLevel{0};
    Vec3 color(1.0f, 0.0f, 1.0f);
    Vertex vertices[] = {
        {Vec3(-1.0f, -1.0f, -1.0f), color, Vec2()}, //down left far
        {Vec3(1.0f, -1.0f, -1.0f), color, Vec2()}, //down right far
        {Vec3(1.0f, 1.0f, -1.0f), color, Vec2()}, //up right far
        {Vec3(-1.0f, 1.0f, -1.0f), color, Vec2()}, //up left far
        {Vec3(-1.0f, -1.0f, 1.0f), color, Vec2()}, //down left near
        {Vec3(1.0f, -1.0f, 1.0f), color, Vec2()}, //down right near
        {Vec3(1.0f, 1.0f, 1.0f), color, Vec2()}, //up right near
        {Vec3(-1.0f, 1.0f, 1.0f), color, Vec2()} //up left near
    };

    //Using GL_LINE_STRIP
    GLushort indices[] = {
        0, 1, 2, 3, 7, 6, 5, 4, 0,
        3, 7, 4, 5, 1, 2, 6
    };
    QMatrix4x4 inverseP = cameraIn->getPerspectiveMatrix().inverted();
    QMatrix4x4 inverseV = cameraIn->getMatrix().inverted();

    //Frustum needs to be moved to initial position of camera
    Vec3 initialPosition = cameraIn->mTransform->getPosition();

    for (int i{0}; i < 8; ++i)
    {
        QVector4D tempVert(vertices[i].get_x(), vertices[i].get_y(), vertices[i].get_z(), 1.0f);
        QVector4D vertexTransposed = inverseV * inverseP * tempVert;
        vertexTransposed = vertexTransposed / vertexTransposed.w();
        //Add the initial position of the camera!
        vertices[i].set_xyz(vertexTransposed.x()+initialPosition.x, vertexTransposed.y()+initialPosition.y, vertexTransposed.z()+initialPosition.z);
    }

    //We now have the vertex and index buffer
    //Now we make a meshbase with boundingVolume of it!
    MeshBase *mMeshbase = new MeshBase();
    mMeshbase->initGeometry(vertices, 8, indices, 16);

    if(lodLevel == 0)
    {
        //Frustum should not have bounding volume
        BoundingVolume *tempBoundingVolume = new BoundingVolume();
        tempBoundingVolume->initGeometry(vertices, 8);
        mMeshbase->setBoundingVolume(tempBoundingVolume);
    }
    lodLevel++;

    Mesh *mMesh = new Mesh(xPos, yPos, zPos, name);
    mMesh->frustrumDraw = true;
    mMesh->initGeometry(mMeshbase);
    //Then we make a meshcomponent and assign the meshbase to it

    return mMesh;
}

GameObject* Mesh_file:: Cube(float xPos, float yPos, float zPos, QString name)
{
    int lodLevel{0};
    Vertex vertices[] = {
        // Vertex data for face 0 - front
        {Vec3(-1.0f, -1.0f,  1.0f), Vec3(0.0f, 0.0f,  1.0f), Vec2(0.0f, 0.0f)},// v0
        {Vec3( 1.0f, -1.0f,  1.0f), Vec3(0.0f, 1.0f,  1.0f), Vec2(0.33f, 0.0f)}, // v1
        {Vec3(-1.0f,  1.0f,  1.0f), Vec3(0.0f, 0.0f,  1.0f), Vec2(0.0f, 0.5f)},// v2
        {Vec3( 1.0f,  1.0f,  1.0f), Vec3(0.0f, 1.0f,  1.0f), Vec2(0.33f, 0.5f)},// v3

        // Vertex data for face 1 - right
        {Vec3( 1.0f, -1.0f,  1.0f), Vec3(1.0f, 1.0f,  1.0f), Vec2( 0.0f, 0.5f)},// v4
        {Vec3( 1.0f, -1.0f, -1.0f), Vec3(1.0f, 0.0f,  0.0f), Vec2(0.33f, 0.5f)},// v5
        {Vec3( 1.0f,  1.0f,  1.0f), Vec3(1.0f, 0.0f,  1.0f), Vec2(0.0f, 1.0f)}, // v6
        {Vec3( 1.0f,  1.0f, -1.0f), Vec3(1.0f, 0.0f,  0.0f), Vec2(0.33f, 1.0f)},  // v7

        // Vertex data for face 2 - back
        {Vec3( 1.0f, -1.0f, -1.0f), Vec3(0.0f, 0.0f,  -1.0f), Vec2(0.66f, 0.5f)},// v8
        {Vec3(-1.0f, -1.0f, -1.0f), Vec3(0.0f, 0.0f,  -1.0f), Vec2(1.0f, 0.5f)},// v9
        {Vec3( 1.0f,  1.0f, -1.0f), Vec3(0.0f, 1.0f,  -1.0f), Vec2(0.66f, 1.0f)}, // v10
        {Vec3(-1.0f,  1.0f, -1.0f), Vec3(0.0f, 0.0f,  -1.0f), Vec2(1.0f, 1.0f)},// v11

        // Vertex data for face 3 - left
        {Vec3(-1.0f, -1.0f, -1.0f), Vec3(-1.0f, 0.0f,  0.0f), Vec2(0.66f, 0.0f)},// v12
        {Vec3(-1.0f, -1.0f,  1.0f), Vec3(-1.0f, 0.0f,  1.0f),Vec2(1.0f, 0.0f)}, // v13
        {Vec3(-1.0f,  1.0f, -1.0f), Vec3(-1.0f, 0.0f,  0.0f), Vec2(0.66f, 0.5f)},// v14
        {Vec3(-1.0f,  1.0f,  1.0f), Vec3(-1.0f, 0.0f,  0.0f), Vec2(1.0f, 0.5f)}, // v15

        // Vertex data for face 4 - bottom
        {Vec3(-1.0f, -1.0f, -1.0f), Vec3(0.0f, -1.0f,  0.0f), Vec2(0.33f, 0.0f)},// v16
        {Vec3( 1.0f, -1.0f, -1.0f), Vec3(0.0f, -1.0f,  0.0f), Vec2(0.66f, 0.0f)},// v17
        {Vec3(-1.0f, -1.0f,  1.0f), Vec3(0.0f, -1.0f,  0.0f), Vec2(0.33f, 0.5f)}, // v18
        {Vec3( 1.0f, -1.0f,  1.0f), Vec3(0.0f, -1.0f,  0.0f),Vec2(0.66f, 0.5f)}, // v19

        // Vertex data for face 5 - top
        {Vec3(-1.0f,  1.0f,  1.0f), Vec3(0.0f, 1.0f,  0.0f), Vec2(0.33f, 0.5f)}, // v20
        {Vec3( 1.0f,  1.0f,  1.0f), Vec3(1.0f, 1.0f,  0.0f), Vec2(0.66f, 0.5f)},// v21
        {Vec3(-1.0f,  1.0f, -1.0f), Vec3(0.0f, 1.0f,  0.0f), Vec2(0.33f, 1.0f)}, // v22
        {Vec3( 1.0f,  1.0f, -1.0f), Vec3(0.0f, 1.0f,  0.0f), Vec2(0.66f, 1.0f)} // v23
    };

    //Using GL_TRIANGLES
    GLushort indices[] = {
        0,  1,  2,  2,  1,  3,      // Face 0 - ( v0,  v1,  v2,  v3)
        4,  5,  6,  6,  5,  7,      // Face 1 - ( v4,  v5,  v6,  v7)
        8,  9, 10, 10, 9, 11,     // Face 2 - ( v8,  v9, v10, v11)
        12, 13, 14, 14,  13, 15,    // Face 3 - (v12, v13, v14, v15)
        16, 17, 18, 18,  17, 19,    // Face 4 - (v16, v17, v18, v19)
        20, 21, 22, 22,  21, 23     // Face 5 - (v20, v21, v22, v23)
    };


    MeshBase *mMeshbase = new MeshBase();
    mMeshbase->initGeometry(vertices, 24, indices, 36);

    if(lodLevel == 0)
    {
        BoundingVolume *tempBoundingVolume = new BoundingVolume();
        tempBoundingVolume->initGeometry(vertices, 24);
        mMeshbase->setBoundingVolume(tempBoundingVolume);
    }
    lodLevel++;
    //Then we make a mesh and assign the meshbase to it
    Mesh *mMesh = new Mesh(xPos, yPos, zPos, name);
    mMesh->frustrumDraw = false;
    mMesh->initGeometry(mMeshbase);

    return mMesh;
}


