#pragma once

#include <QMainWindow>
#include <QActionGroup>
#include <QtWidgets>

class Viewer : public QMainWindow {
public:
	explicit Viewer();
	void createMenus();
	void addFile();
	bool openFile(const QString& fileName);
private:

	QAction* m_openFile;

};