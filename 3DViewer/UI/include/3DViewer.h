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
public slots:
    void loadObject();
protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    bool openFile(const QString& file);

    void connectSignalsSlots();
    void createStatusBar();

    OpenGLRenderer* m_openGLRenderer;
    Ui::Viewer *ui;

    QLabel* m_mousePosLbl;
    QLabel* m_framerateLbl;
    QLabel* m_statusLbl;
};

