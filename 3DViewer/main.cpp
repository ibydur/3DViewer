#include <QApplication>
#include "UI/include/3DViewer.h"

int main(int argc, char *argv[]) {

	QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(Viewer::tr("3DViewer"));

    Viewer viewer;
    viewer.show();

    return app.exec();
}   