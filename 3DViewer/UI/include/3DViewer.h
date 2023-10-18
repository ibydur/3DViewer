#pragma once

#include <QMainWindow>
#include <QtWidgets>

#include "../../Renderer/include/OpenGLRenderer.h"
#include "../../Scene/include/SceneObject.h"

class Viewer : public QMainWindow {
public:
	explicit Viewer();
	void createMenus();
	void addFile();
	bool openFile(const QString& fileName);
private:
	OpenGLRenderer* m_openGLRenderer;
	QAction* m_openFile;
};