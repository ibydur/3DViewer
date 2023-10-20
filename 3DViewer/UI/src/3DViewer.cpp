#include "../../UI/include/3DViewer.h"
#include "../../UI/include/ui_3DViewer.h"

#include <QFileDialog>

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer),
    //status bar labels
    m_mousePosLbl (new QLabel(this)),
    m_framerateLbl(new QLabel(this)),
    m_statusLbl   (new QLabel(this))
{
    ui->setupUi(this);
    m_openGLRenderer = new OpenGLRenderer(ui->openGLWidget);
    connectSignalsSlots();
    createStatusBar();
}

Viewer::~Viewer()
{
    delete ui;
}

void Viewer::resizeEvent(QResizeEvent* event)
{
    ui->openGLWidget->resize(event->size().width()-200, event->size().height());
    m_openGLRenderer->resize(event->size().width()-200, event->size().height());
}

bool Viewer::openFile(const QString& file)
{
    auto mesh = CGAL_API::constructMeshFromObj(file.toStdString());
    if (nullptr == mesh) {
        qCritical() << "Something went wrong while loading obj. Mesh wasn't constructed.";
        return false;
    }
    m_openGLRenderer->addObject(std::move(SceneObject::makeObject(mesh)));
    return true;
}

void Viewer::connectSignalsSlots()
{
    //menu action
    connect(ui->actionOpen, &QAction::triggered, this, &Viewer::loadObject);

    //status bar
    connect(m_openGLRenderer, &OpenGLRenderer::mouseMoved, m_mousePosLbl, &QLabel::setText);
    connect(m_openGLRenderer, &OpenGLRenderer::framerateUpdated, m_framerateLbl, &QLabel::setText);
    connect(m_openGLRenderer, &OpenGLRenderer::sceneStatusUpdated, m_statusLbl, &QLabel::setText);
}

void Viewer::createStatusBar()
{
    statusBar()->addWidget(new QLabel("Status:", this));
    statusBar()->addWidget(m_statusLbl);

    statusBar()->addWidget(new QLabel("Mouse position:", this));
    statusBar()->addWidget(m_mousePosLbl);

    statusBar()->addWidget(new QLabel("Framerate:", this));
    statusBar()->addWidget(m_framerateLbl);
}

void Viewer::loadObject()
{
    QFileDialog dialog(this, tr("Open File"));
    dialog.setNameFilter("*.obj");

    while (dialog.exec() == QDialog::Accepted) {
        if (openFile(dialog.selectedFiles().first())) {
            break;
        }
    }
}