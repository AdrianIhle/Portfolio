#include "editorwindow.h"
#include "ui_EditorWindow.h"
#include "gameobject.h"
#include "makemesh.h"
#include "constants.h"

#include <QFileDialog>

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
    connect(ui->RenderWindow, SIGNAL(initHierarchy(std::vector<GameObject*>&)), this, SLOT(hierarchyInit(std::vector<GameObject*>&)));
}

EditorWindow::~EditorWindow()
{
    //ui->RenderWindow->cleanup();
    delete ui;
}

void EditorWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void EditorWindow::on_actionLoad_Mesh_triggered()
{
    qDebug() << "Open Mesh says EditorWindow!";
}

void EditorWindow::on_ColorButton_released()
{
    qDebug() << "Test pressed";
    ui->RenderWindow->setCameraColor();
}

void EditorWindow::hierarchyInit(std::vector<GameObject*> &mGeometry)
{
    ui->HierarchyList->clear();
    foreach (auto &tempGameObject, mGeometry) {
        ui->HierarchyList->addItem(tempGameObject->getName());
    }
}



void EditorWindow::on_Cube_clicked()
{
    ui->RenderWindow->addCube();
}

void EditorWindow::on_Ball_clicked()
{
    ui->RenderWindow->addBall();
}

void EditorWindow::on_Plane_clicked()
{
    ui->RenderWindow->addPlane();
}



void EditorWindow::on_ReadObjFile_clicked()
{
    ui->RenderWindow->readFromFile("none", "test");
}

void EditorWindow::on_ToggleCamera_pressed()
{
    if(ui->RenderWindow->editorCamera == false)
        ui->RenderWindow->editorCamera = true;
    else
        ui->RenderWindow->editorCamera = false;
}

void EditorWindow::on_HierarchyList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    currentObject = ui->RenderWindow->ObjectFromList(ui->HierarchyList->row(current));
    UpdateInspector();
    qDebug() << "changing item" << endl;

}

void EditorWindow::UpdatePosition()
{
    currentObject->mTransform->setPosition(ui->InspectPosX->text().toFloat(), ui->InspectPosY->text().toFloat(), ui->InspectPosZ->text().toFloat());
}

void EditorWindow::UpdateRotation()
{
    currentObject->mTransform->setRotation(ui->InspectRotX->text().toFloat(), ui->InspectRotY->text().toFloat(), ui->InspectRotZ->text().toFloat());
}

void EditorWindow::UpdateScale()
{
    currentObject->mTransform->setScale(ui->InspectScaX->text().toFloat(), ui->InspectScaY->text().toFloat(), ui->InspectScaZ->text().toFloat());
}

void EditorWindow::UpdateInspector()
{
    ui->ObjName->setText(currentObject->getName());

    ui->InspectPosX->setText(QString::number(currentObject->mTransform->getPosition().x));
    ui->InspectPosY->setText(QString::number(currentObject->mTransform->getPosition().y));
    ui->InspectPosZ->setText(QString::number(currentObject->mTransform->getPosition().z));

    ui->InspectRotX->setText(QString::number(currentObject->mTransform->getRotation().x));
    ui->InspectRotY->setText(QString::number(currentObject->mTransform->getRotation().y));
    ui->InspectRotZ->setText(QString::number(currentObject->mTransform->getRotation().z));

    ui->InspectScaX->setText(QString::number(currentObject->mTransform->getScale().x));
    ui->InspectScaY->setText(QString::number(currentObject->mTransform->getScale().y));
    ui->InspectScaZ->setText(QString::number(currentObject->mTransform->getScale().z));
}

void EditorWindow::on_ObjName_editingFinished()
{
    currentObject->setName(ui->ObjName->text());
    ui->HierarchyList->currentItem()->setText(ui->ObjName->text());
    UpdateInspector();
}

void EditorWindow::on_InspectPosX_editingFinished()
{
    UpdatePosition();
    UpdateInspector();
}

void EditorWindow::on_InspectPosY_editingFinished()
{
    UpdatePosition();
    UpdateInspector();
}

void EditorWindow::on_InspectPosZ_editingFinished()
{
    UpdatePosition();
    UpdateInspector();
}

void EditorWindow::on_InspectRotX_editingFinished()
{
    UpdateRotation();
    UpdateInspector();
}

void EditorWindow::on_InspectRotY_editingFinished()
{
    UpdateRotation();
    UpdateInspector();
}

void EditorWindow::on_InspectRotZ_editingFinished()
{
    UpdateRotation();
    UpdateInspector();
}

void EditorWindow::on_InspectScaX_editingFinished()
{
    UpdateScale();
    UpdateInspector();
}

void EditorWindow::on_InspectScaY_editingFinished()
{
    UpdateScale();
    UpdateInspector();
}

void EditorWindow::on_InspectScaZ_editingFinished()
{
    UpdateScale();
    UpdateInspector();
}


void EditorWindow::on_SaveScene_clicked()
{
    QString filter = "*.scene.txt";
    QString filepath = QFileDialog::getSaveFileName(this, tr("fileTitle"),Orf::assetFilePath ,filter, &filter);
    ui->RenderWindow->SaveScene(filepath, ui->RenderWindow->mScene);
}

void EditorWindow::on_LoadScene_clicked()
{
    QString filter = "*.scene.txt";
    QString mFilePath = QFileDialog::getOpenFileName(this, tr("fileTitle"),Orf::assetFilePath ,filter, &filter);
    std::string filepath = mFilePath.toStdString();
    ui->RenderWindow->LoadScene(filepath, ui->RenderWindow->mScene);
}


void EditorWindow::on_LoD_textEdited(const QString &arg1)
{
    int temp = arg1.toInt();
  /*  if (temp < 3 && temp > -2)  //must be between -1 and 2
        ui->RenderWindow->setLodLevel(arg1.toInt());
    else
    {
        ui->RenderWindow->setLodLevel(-1);
        ui->lodlevelEDIT->setText("-1");
    }*/

}

void EditorWindow::on_ToggleCamera_clicked()
{
        ui->RenderWindow->editorCamera != ui->RenderWindow->editorCamera;

}


void EditorWindow::on_lineEdit_textEdited(const QString &arg1)
{
    unsigned int temp = arg1.toFloat();
    temp = ui->RenderWindow->getFrameRate();
    ui->lineEdit->setText(QString::number(temp));
}

void EditorWindow::on_lineEdit_2_textEdited(const QString &arg1)
{


    unsigned int temp = arg1.toFloat();
    temp = ui->RenderWindow->verticesDrawn;
    ui->lineEdit_2->setText(QString::number(temp));
}

void EditorWindow::setRenderInfo(unsigned int vertexInfo, unsigned int msInfo)
{
    ui->lineEdit->setText(QString().number(vertexInfo));

        ui->lineEdit_2->setText(QString().number(msInfo));
}
