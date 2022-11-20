#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include "editorwindow.h"
#endif

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QSurfaceFormat format;

    format.setVersion( 2 , 0 );
    format.setSamples(4);    // Set the number of samples used for multisampling

    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    QSurfaceFormat::setDefaultFormat( format );

    app.setApplicationName("ORF 2016");
    app.setApplicationVersion("0.1");

#ifndef QT_NO_OPENGL
	EditorWindow editorWindow;
    editorWindow.resize(1280, 720);
    editorWindow.show();
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}
