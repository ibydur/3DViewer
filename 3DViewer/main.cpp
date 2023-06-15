#include <QApplication>

int main(int argc, char *argv[]) {

	QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName("3DViewer");
    return app.exec();
}