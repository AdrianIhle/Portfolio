#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H
#include "gameengine.h"
#include "QListWidgetItem"
#include <QMainWindow>
#include<QKeyEvent>

namespace Ui {
class EditorWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    GameObject* target;
    ~EditorWindow();

    void setColor();

private slots:

    void on_actionQuit_triggered();

    void on_actionLoad_Mesh_triggered();

    void on_ColorButton_released();

    void on_Cube_clicked();

    void on_Ball_clicked();

    void on_Plane_clicked();

    void on_ReadObjFile_clicked();

    void on_ToggleCamera_pressed();

    void on_HierarchyList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_ObjName_editingFinished();

    void on_InspectPosX_editingFinished();

    void on_InspectPosY_editingFinished();

    void on_InspectPosZ_editingFinished();

    void on_InspectRotX_editingFinished();

    void on_InspectRotY_editingFinished();

    void on_InspectRotZ_editingFinished();

    void on_InspectScaX_editingFinished();

    void on_InspectScaY_editingFinished();

    void on_InspectScaZ_editingFinished();

    void on_SaveScene_clicked();

    void on_LoadScene_clicked();

    void on_LoD_textEdited(const QString &arg1);

    void on_ToggleCamera_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void setRenderInfo(unsigned int vertexInfo, unsigned int msInfo);

public slots:
    void hierarchyInit(std::vector<class GameObject*> &mGeometry);
    void UpdateInspector();

private:
    Ui::EditorWindow *ui;
    GameObject* currentObject;
    void UpdatePosition();
    void UpdateRotation();
    void UpdateScale();

    int newname{0};

    int newnameBall{0};

    int newnamePlane{0};

    class GameObject *mSelected{nullptr};

    class Mesh_file *mSelectedMesh{nullptr};

    bool added = false;

    std::vector<GameObject*> mDisplayObjects;//vector just to display the objects

    bool ok;
    double d = 0;
    double e = 0;
    double f = 0;





};

#endif // EDITORWINDOW_H
