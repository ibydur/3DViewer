#pragma once

#include <QMainWindow>
#include <QLabel>

#include "../../Renderer/include/OpenGLRenderer.h"
#include "../../Scene/include/SceneObject.h"

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
    void loadObject();
signals:
    void sceneUpdated(const std::shared_ptr<SceneObject>&);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    bool openFile(const QString& file);

    void connectSignalsSlots();
    void createStatusBar();

    OpenGLRenderer* m_openGLRenderer;
    Ui::Viewer *ui;

    //status bar
    QLabel* m_mousePosLbl;
    QLabel* m_framerateLbl;
    QLabel* m_statusLbl;
};

