#pragma once

#include <QMainWindow>
#include <QtConcurrent>
#include <QLabel>

#include "OpenGLRenderer.h"
#include "SceneObject.h"

namespace Ui {
class Viewer;
}

class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = nullptr);
    ~Viewer();
    void addFileToTreeList(const QString& file, unsigned int objId);

public slots:
    void openFile();

signals:
    void sceneUpdated(const std::shared_ptr<SceneObject>&);
    void objectRemoved();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void handleObjectConstruction();
    void authorInfo();
    void hotkeysInfo();
private:
    std::shared_ptr<SceneObject> constructObject(const QString& file);
    void connectSignalsSlots();
    void createStatusBar();
    void handleObjectRemovement();

    OpenGLRenderer* m_openGLRenderer;
    Ui::Viewer *ui;
    Scene m_scene;
    //status bar
    QLabel* m_mousePosLbl;
    QLabel* m_framerateLbl;
    QLabel* m_statusLbl;
    QLabel* m_drawingModeLbl;

    QFutureWatcher<std::shared_ptr<SceneObject>> watcher;
};

