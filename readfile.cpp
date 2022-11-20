#include "readfile.h"
#include <QDebug>
#include "constants.h"

ReadFile::ReadFile(std::string filePath)
{
    mFileName =filePath;
}

void ReadFile::ReadFileToMesh()
{
    int nrOfVerticies = 0;
    int nrOfUV = 0;
    int nrOfNormals = 0;
    int nrOfFaces = 0;

    std::ifstream mFileIn(mFileName);

    while(getline(mFileIn, mOneLine))
    {

        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << mOneLine;
        //Streaming one word out of line
        sStream >> mOneWord;

        if (mOneWord == "#")
        {
            continue;
        }
        if (mOneWord == "v")
        {
            nrOfVerticies++;
            continue;
        }
        if (mOneWord == "vt")
        {
            nrOfUV++;
            continue;
        }
        if (mOneWord == "vn")
        {
            nrOfNormals++;
            continue;
        }
        if (mOneWord == "f")
        {
            nrOfFaces++;
            continue;
        }

        qDebug() << "Line was something strange! "  << endl;

    }

    nVerticies = new Vertex [nrOfVerticies];


    while(getline(mFileIn, mOneLine))
    {
        int vRead = 0;
        int  uRead = 0;
        int nRead = 0;

        Vec3 nXYZ;
        GLfloat nS;
        GLfloat nT;
        Vec3 nNormal;


        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << mOneLine;
        //Streaming one word out of line
        sStream >> mOneWord;

        if (mOneWord == "#")
        {
            continue;
        }
        if (mOneWord == "v")
        {
            vRead++;


            for (int i = 0; i < 4; ++i)
            {
                sStream >> mOneWord;
                if (mOneWord != "v")
                {
                    float intCast = std::stof(mOneWord);
                    if(i == 1)
                        nXYZ.x = intCast;
                    if(i ==2)
                        nXYZ.y = intCast;
                    if(i ==3)
                        nXYZ.z = intCast;
                }

            }
            nVerticies[vRead].set_xyz(nXYZ);

            continue;

        }
        if (mOneWord == "vt")
        {
            uRead++;
            for (int i = 0; i < 2; ++i)
            {
                sStream >> mOneWord;
                if (mOneWord != "vt")
                {
                    float intCast = std::stof(mOneWord);
                    if(i ==1)
                        nS = intCast;
                    if(i ==2)
                        nT = intCast;

                }

            }
            nVerticies[vRead].set_st(nS,nT);
            continue;
        }
        if (mOneWord == "vn")
        {
            nRead++;
            for (int i = 0; i < 4; ++i)
            {
                sStream >> mOneWord;
                if (mOneWord != "vn")
                {
                    float intCast = std::stof(mOneWord);
                    if(i ==1)
                        nNormal.x = intCast;
                    if(i ==1)
                        nNormal.y = intCast;
                    if(i ==1)
                        nNormal.z = intCast;

                }

            }
            nVerticies[vRead].set_normal(nNormal);
            continue;
        }
        if (mOneWord == "f")
        {
            qDebug() << "Line was a face " << endl;


            //            qDebug() << "Line is a face "  << mOneWord << " ";
            //            for (int i = 0; i < 3; ++i)
            //            {
            //                sStream >> mOneWord;
            //                qDebug() << mOneWord << ", ";
            //            }
            //            qDebug() << endl;
            //            continue;
        }

        qDebug() << "Line was something strange! " << endl;
   }

    //being a nice boy and closing the file after use
    mFileIn.close();

}



