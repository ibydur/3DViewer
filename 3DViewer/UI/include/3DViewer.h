#pragma once

#include <QMainWindow>
#include <QActionGroup>
#include <QtWidgets>
#include "../../Renderer/include/OpenGLRenderer.h"

class Viewer : public QMainWindow {
public:
	explicit Viewer();
	void createMenus();
	void addFile();
	bool openFile(const QString& fileName);
private:
	OpenGLRenderer* openGLRenderer;
	QAction* m_openFile;

};