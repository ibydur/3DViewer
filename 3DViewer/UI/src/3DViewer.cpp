#include "../../UI/include/3DViewer.h"
#include "../../UI/include/ui_3DViewer.h"

#include <QFileDialog>

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer),
    m_scene(),
    //status bar labels
    m_mousePosLbl (new QLabel(this)),
    m_framerateLbl(new QLabel(this)),
    m_statusLbl   (new QLabel(this))
{
    ui->setupUi(this);
    m_openGLRenderer = new OpenGLRenderer(ui->openGLWidget, m_scene);
    connectSignalsSlots();
    createStatusBar(); 
}

Viewer::~Viewer()
{
    delete ui;
}

void Viewer::addFileToTreeList(const QString& file, unsigned int objId)
{
    auto item = new QListWidgetItem(file);
    item->setData(Qt::UserRole, objId);
    ui->objsListWidget->addItem(item);
    ui->objsListWidget->setCurrentItem(item);
}

void Viewer::resizeEvent(QResizeEvent* event)
{
    m_openGLRenderer->setFixedSize(ui->openGLWidget->size());
}

void Viewer::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Delete:
        emit objectRemoved();
        handleObjectRemovement();
        m_openGLRenderer->update();
        break;
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    }
}

void Viewer::handleObjectRemovement()
{
    QListWidgetItem* selectedItem = ui->objsListWidget->currentItem();
    delete selectedItem;
}

std::shared_ptr<SceneObject> Viewer::constructObject(const QString& file)
{
    std::unique_ptr<Surface_mesh> mesh = CGAL_API::constructMeshFromObj(file.toStdString());
    if (nullptr == mesh) {
        qCritical() << "Something went wrong while loading obj. Mesh wasn't constructed well.";
    }
    std::shared_ptr<SceneObject> obj = SceneObject::makeObject(QFileInfo(file), mesh);
    if (nullptr == obj) {
        qCritical() << "Something went wrong while loading obj. Scene object wasn't constructed well.";
    }
    return obj;
}

void Viewer::connectSignalsSlots()
{
    //menu action
    connect(ui->actionOpen, &QAction::triggered, this, &Viewer::openFile);
    connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);

    //details frame
    connect(&m_scene, &Scene::verticesUpdated, ui->objDataVerticesLbl, &QLabel::setText);
    connect(&m_scene, &Scene::facesUpdated,    ui->objDataFacesLbl,    &QLabel::setText);
    connect(&m_scene, &Scene::edgesUpdated,    ui->objDataEdgesLbl,    &QLabel::setText);
    connect(&m_scene, &Scene::IdUpdated,       ui->objDataIdLbl,       &QLabel::setText);
    connect(&m_scene, &Scene::widthUpdated,    ui->objDataWidthLbl,    &QLabel::setText);
    connect(&m_scene, &Scene::heightUpdated,   ui->objDataHeightLbl,   &QLabel::setText);
    connect(&m_scene, &Scene::lengthUpdated,   ui->objDataLengthLbl,   &QLabel::setText);
    connect(&m_scene, &Scene::nameUpdated,     ui->objDataNameLbl,     &QLabel::setText);

    //status bar
    connect(m_openGLRenderer, &OpenGLRenderer::mouseMoved,         m_mousePosLbl,       &QLabel::setText);
    connect(m_openGLRenderer, &OpenGLRenderer::framerateUpdated,   m_framerateLbl,      &QLabel::setText);

    connect(&watcher, &QFutureWatcher<std::shared_ptr<SceneObject>>::finished,  this, &Viewer::handleObjectConstruction);
    connect(&watcher, &QFutureWatcher<std::shared_ptr<SceneObject>>::started,  [this]() { m_statusLbl->setText("\"" + watcher.property("filename").toString() + "\" is loading"); });
    connect(&watcher, &QFutureWatcher<std::shared_ptr<SceneObject>>::finished, [this]() { m_statusLbl->setText(""); });

    //scene
    connect(ui->objVisibleCB, &QCheckBox::stateChanged, &m_scene, &Scene::setCurrentObjVisibility);
    connect(this, &Viewer::sceneUpdated, &m_scene, &Scene::addObjectOnScene);
    connect(this, &Viewer::objectRemoved, &m_scene, &Scene::removeCurrentObjSelection);
    connect(ui->objsListWidget, &QListWidget::currentItemChanged, &m_scene, &Scene::handleSceneItemChanged);
}

void Viewer::createStatusBar()
{
    statusBar()->addWidget(m_statusLbl);

    statusBar()->addWidget(new QLabel("Mouse position:", this));
    statusBar()->addWidget(m_mousePosLbl);

    statusBar()->addWidget(new QLabel("Framerate:", this));
    statusBar()->addWidget(m_framerateLbl);
}

void Viewer::handleObjectConstruction()
{
    const auto& obj = std::move(watcher.result());
    addFileToTreeList(obj->getFilePath(), obj->getID());
    emit sceneUpdated(obj);
}

void Viewer::openFile()
{
    QFileDialog dialog(this, tr("Open File"));
    dialog.setNameFilter("*.obj");

    if (dialog.exec() == QDialog::Accepted) {           
        watcher.setProperty("filename", dialog.selectedFiles().first());
        watcher.setFuture(QtConcurrent::run(this, &Viewer::constructObject, dialog.selectedFiles().first()));
    }    
}