#include "../include/3DViewer.h"
#include <QSizePolicy>

Viewer::Viewer() {
	createMenus();
	OpenGLRenderer* openGLWidget = new OpenGLRenderer(this);
	setFocus();
	openGLWidget->setFocusPolicy(Qt::StrongFocus);
	setCentralWidget(openGLWidget);
	resize(QGuiApplication::primaryScreen()->availableSize());
}

void Viewer::createMenus() {
	//file menu
	{
		auto fileMenu = menuBar()->addMenu(tr("&File"));
		m_openFile = fileMenu->addAction(tr("&Open file..."), this, &Viewer::addFile);
		fileMenu->addSeparator();
		auto exitAction = fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
		fileMenu->addSeparator();
	}
	//about menu
	{

	}
}

void Viewer::addFile() {
	QFileDialog dialog(this, tr("Open File"));
	dialog.setNameFilter("*.obj");

	while (dialog.exec() == QDialog::Accepted)
	{
		if (openFile(dialog.selectedFiles().first()))
			break;
	}
}

bool Viewer::openFile(const QString& fileName) {
	return true;
}