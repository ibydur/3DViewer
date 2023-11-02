#include "3DViewer.h"
#include "ui_3DViewer.h"

#include <QFileDialog>
#include <QMessageBox>

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer),
    m_scene(),
    //status bar labels
    m_mousePosLbl   (new QLabel("x = 0, y = 0", this)),
    m_framerateLbl  (new QLabel("0.00", this)),
    m_drawingModeLbl(new QLabel("solid", this)),
    m_statusLbl     (new QLabel(this))
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
    if (nullptr != mesh) {
        std::shared_ptr<SceneObject> obj = SceneObject::makeObject(QFileInfo(file), mesh);
        return obj;
    }
    return nullptr;
}

void Viewer::connectSignalsSlots()
{
    //file menu
    connect(ui->actionOpen,    &QAction::triggered, this, &Viewer::openFile);
    connect(ui->actionExit,    &QAction::triggered, this, &QApplication::quit);
    //help menu
    connect(ui->actionAuthor,  &QAction::triggered, this, &Viewer::authorInfo);
    connect(ui->actionHotkeys, &QAction::triggered, this, &Viewer::hotkeysInfo);

    //details frame
    connect(&m_scene, &Scene::verticesUpdated, ui->objDataVerticesLbl, &QLabel::setText);
    connect(&m_scene, &Scene::facesUpdated,    ui->objDataFacesLbl,    &QLabel::setText);
    connect(&m_scene, &Scene::edgesUpdated,    ui->objDataEdgesLbl,    &QLabel::setText);
    connect(&m_scene, &Scene::IdUpdated,       ui->objDataIdLbl,       &QLabel::setText);
    connect(&m_scene, &Scene::widthUpdated,    ui->objDataWidthLbl,    &QLabel::setText);
    connect(&m_scene, &Scene::heightUpdated,   ui->objDataHeightLbl,   &QLabel::setText);
    connect(&m_scene, &Scene::lengthUpdated,   ui->objDataLengthLbl,   &QLabel::setText);
    connect(&m_scene, &Scene::nameUpdated,     ui->objDataNameLbl,     &QLabel::setText);

    //renderer actions
    connect(&m_scene, &Scene::redrawRenderer, m_openGLRenderer, &OpenGLRenderer::redraw);
    connect(&m_scene, &Scene::updateCamera,   m_openGLRenderer, &OpenGLRenderer::updateCamera);

    //status bar
    connect(m_openGLRenderer, &OpenGLRenderer::mouseMoved,         m_mousePosLbl,       &QLabel::setText);
    connect(m_openGLRenderer, &OpenGLRenderer::framerateUpdated,   m_framerateLbl,      &QLabel::setText);
    connect(m_openGLRenderer, &OpenGLRenderer::drawingModeChanged, m_drawingModeLbl,    &QLabel::setText);

    //loading obj
    connect(&watcher, &QFutureWatcher<std::shared_ptr<SceneObject>>::finished,  this, &Viewer::handleObjectConstruction);
    connect(&watcher, &QFutureWatcher<std::shared_ptr<SceneObject>>::started,  [this]() { m_statusLbl->setText("\"" + watcher.property("filename").toString() + "\" is loading"); });
    connect(&watcher, &QFutureWatcher<std::shared_ptr<SceneObject>>::finished, [this]() { m_statusLbl->setText(""); });

    //scene
    connect(ui->objVisibleCB,            &QCheckBox::stateChanged,         &m_scene, &Scene::setCurrentObjVisibility);
    connect(ui->objDataMaterialComboBox, &QComboBox::currentTextChanged,   &m_scene, &Scene::setCurrentMaterial);
    connect(ui->objsListWidget,          &QListWidget::currentItemChanged, &m_scene, &Scene::handleSceneItemChanged);
    connect(this,                        &Viewer::sceneUpdated,            &m_scene, &Scene::addObjectOnScene);
    connect(this,                        &Viewer::objectRemoved,           &m_scene, &Scene::removeCurrentObjSelection);
}

void Viewer::createStatusBar()
{
    statusBar()->addWidget(m_statusLbl);

    statusBar()->addWidget(new QLabel("Mouse position:", this));
    statusBar()->addWidget(m_mousePosLbl);

    statusBar()->addWidget(new QLabel("Framerate:", this));
    statusBar()->addWidget(m_framerateLbl);

    statusBar()->addWidget(new QLabel("Drawing mode:", this));
    statusBar()->addWidget(m_drawingModeLbl);
}

void Viewer::handleObjectConstruction()
{
    const auto& obj = std::move(watcher.result());
    if (nullptr != obj) {
        emit sceneUpdated(obj);
        addFileToTreeList(obj->getFilePath(), obj->getID()); 
    }
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

void Viewer::hotkeysInfo()
{
    QString text =
        "Camera:\n"
        "    W, S, A, D             \tmove in free camera mode\n"
        "    RButton                \tpan in free camera mode\n"
        "    Wheel                  \tzoom\n"
        "\nScene:\n"
        "    LButton                \tobject rotation\n"
        "    LButton + RButton      \tobject translation\n"
        "    C                      \t\tswitch drawing mode\n"
        "    R                      \t\treset object and camera position\n"
        "\nApplication:\n"
        "    ESC                    \t\tclose app\n"
        "    DELETE                 \tremove current selected object";

    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setWindowTitle(tr("Hotkeys Info"));
    msgBox->setText(text);
    msgBox->setModal(false);
    msgBox->show();
}

void Viewer::authorInfo()
{
    QString text =
        "Name:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ilya Budarov<br>"
        "Mail:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"mailto:budarov12345@gmail.com\">budarov12345@gmail.com</a><br>"
        "Github:&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"https://github.com/ibydur\">https://github.com/ibydur</a><br>"
        "LinkedIn:&nbsp;<a href=\"https://www.linkedin.com/in/ilya-budarov-073ab720a/\">https://www.linkedin.com/in/ilya-budarov-073ab720a/</a><br>";

    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setWindowTitle(tr("Author Info"));
    msgBox->setText(text);
    msgBox->setModal(false);
    msgBox->show();
}