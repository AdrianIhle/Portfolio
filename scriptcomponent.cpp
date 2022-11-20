#include "scriptcomponent.h"
#include "constants.h"
#include <QFile>
#include <QJSEngine>
#include "transform.h"

ScriptComponent::ScriptComponent(GameObject *owner, QString scriptFileNameIn)
    :Component(owner), mScriptFileName(scriptFileNameIn)
{
    initScriptFile();
    QJSValue objectTest = mJSEngine.newQObject(this);
    mJSEngine.globalObject().setProperty("myObject", objectTest);
}

void ScriptComponent::update()
{
    QJSValue func = mJSEngine.evaluate("initMove");
    QJSValue result = func.call();

    if (result.isError())
        qDebug()
                << "Uncaught exception at line"
                << result.property("lineNumber").toInt()
                << ":" << result.toString();
}

void ScriptComponent::move()
{
    Vec3 newPos(0.1f, 0,0);
    Vec3 OldPos = mOwner->mTransform->getPosition();
    mOwner->mTransform->setPosition(newPos + OldPos);
}

void ScriptComponent::initScriptFile()
{
    qDebug()<<mScriptFileName;
    QString fileName = Orf::assetFilePath + mScriptFileName;
    //Make a QFile for it
    QFile scriptFile(fileName);

    //Try to open file and give error if something is wrong
    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - NO FILE HERE: " << fileName;
    //    else
    //        qDebug() << fileName << "opened";

    //reads the file
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    //close the file, because we don't need it anymore
    scriptFile.close();
    //    qDebug() << contents;

    //Loads the whole script into script engine:
    //The important part! fileName is used to report bugs in the file
    mJSEngine.evaluate(contents, fileName);
}
