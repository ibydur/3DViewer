#include "../include/3DViewer.h"

Viewer::Viewer() :
	m_openGLRenderer(new OpenGLRenderer(this))
{
	m_openGLRenderer->setFocusPolicy(Qt::StrongFocus);
	m_openGLRenderer->setFocus();

	setCentralWidget(m_openGLRenderer);
	resize(QGuiApplication::primaryScreen()->availableSize());

	createMenus();
}

void Viewer::createMenus() {
	//file menu
	auto fileMenu = menuBar()->addMenu(tr("&File"));
	m_openFile = fileMenu->addAction(tr("&Open file..."), this, &Viewer::addFile);
	fileMenu->addSeparator();
	auto exitAction = fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
	fileMenu->addSeparator();
	//about menu

}

void Viewer::addFile() {
	QFileDialog dialog(this, tr("Open File"));
	dialog.setNameFilter("*.obj");

	while (dialog.exec() == QDialog::Accepted) {
		if (openFile(dialog.selectedFiles().first()))
			break;
	}
}

bool Viewer::openFile(const QString& fileName) {
	auto mesh = CGAL_API::constructMeshFromObj(fileName.toStdString());
	if (nullptr == mesh) {
		return false;
	}
	m_openGLRenderer->addObject(std::move(SceneObject::makeObject(mesh)));
	m_openGLRenderer->update();
	return true;
}