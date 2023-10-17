#include <QDir>
#include <QEvent.h>
#include <QApplication>

#include "../include/OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer(QWidget* parent) :
    QOpenGLWidget(parent),
    drawingMode(Mode::SOLID),
    m_camera(),
    m_scene()
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

OpenGLRenderer::~OpenGLRenderer()
{
    for (const auto& obj : m_scene.getObjectsLst()) {
        obj->release();
    }
}

void OpenGLRenderer::addObject(const std::shared_ptr<SceneObject>& obj)
{
    m_scene.addObjectOnScene(obj);
}

void OpenGLRenderer::drawObject(SceneObject& obj)
{
    obj.vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, obj.vertices.size());
    obj.vao.release();
}

void OpenGLRenderer::initObjectBuffers(SceneObject& obj)
{
    obj.vao.create();
    obj.vao.bind();

    obj.vbo.create();
    obj.vbo.setUsagePattern(QOpenGLBuffer::StreamDraw);
    obj.vbo.bind();

    obj.vbo.allocate(obj.vertices.constData(), obj.vertices.size() * sizeof(QVector3D));

    m_shader_program->bind();
    m_shader_program->enableAttributeArray(0);
    m_shader_program->setAttributeArray(0, GL_FLOAT, 0, 3);

    obj.setBuffersInited(true);
}

void OpenGLRenderer::initializeGL() {
    timer.start();
    initializeOpenGLFunctions();
    initializeShaders();
    m_last_mouse_pos = QPoint(width() / 2.0f, height() / 2.0f);
}

void OpenGLRenderer::paintGL() {
    qreal elapsedTimeSeconds = static_cast<qreal>(timer.elapsed()) / 1000.0;
    deltaTime = elapsedTimeSeconds - lastFrame;
    lastFrame = elapsedTimeSeconds;
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Use the shader program
    m_shader_program->bind();
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getZoom(), static_cast<float>(width() / height()), 0.1f, 100.0f);
    m_view.setToIdentity();
    m_view = m_camera.getViewMatrix();
    m_model.setToIdentity();
    
    (drawingMode == Mode::SOLID) ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (const auto& obj : m_scene.getObjectsLst()) {
        if (!obj->isBuffersInited()) {
            obj->intializeBuffers(this);
        }
        auto objCenter = obj->getObjectCenter();
        m_model.translate(-objCenter);
        m_shader_program->setUniformValue("viewMatrix", m_view);
        m_shader_program->setUniformValue("projectionMatrix", m_projection);
        m_shader_program->setUniformValue("modelMatrix", m_model);
        obj->draw(this);
    }
}

void OpenGLRenderer::initializeShaders() {
    QString current_source_filepath = QString::fromUtf8(__FILE__); // Full path to the current source file
    QDir source_file_dir = QFileInfo(current_source_filepath).dir(); // Directory of the source file
    QDir shaders_dir(source_file_dir.absoluteFilePath("../shaders")); // Path to shaders directory

    bool compile_result;
    m_vertex_shader   = new QOpenGLShader(QOpenGLShader::Vertex, this);
    compile_result    = m_vertex_shader->compileSourceFile(shaders_dir.filePath("main_vert.glsl")); // Path to vertex shader file
    m_fragment_shader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    compile_result    = m_fragment_shader->compileSourceFile(shaders_dir.filePath("main_frag.glsl")); // Path to fragment shader file

    // Create and link shader program
    m_shader_program = new QOpenGLShaderProgram(this);
    m_shader_program->addShader(m_vertex_shader);
    m_shader_program->addShader(m_fragment_shader);
    m_shader_program->link(); 

    delete m_vertex_shader;
    delete m_fragment_shader;
}

void OpenGLRenderer::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        QApplication::quit();
    case Qt::Key_W:
        m_camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
        break;
    case Qt::Key_S:
        m_camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
        break;
    case Qt::Key_A:
        m_camera.processKeyboard(CameraMovement::LEFT, deltaTime);
        break;
    case Qt::Key_D:
        m_camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
        break;
    case Qt::Key_R:
        m_camera.reset();
        break;
    case Qt::Key_C:
        drawingMode = (drawingMode == Mode::SOLID) ? Mode::WIREFRAME : Mode::SOLID;
        break;
    }
    update();
}

void OpenGLRenderer::mousePressEvent(QMouseEvent* event)
{

}

void OpenGLRenderer::mouseReleaseEvent(QMouseEvent* event)
{
    mFirstTimeHandled = true;
    update();
}

void OpenGLRenderer::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        if (mFirstTimeHandled)
        {
            mFirstTimeHandled = false;
            m_last_mouse_pos.setX(event->globalX());
            m_last_mouse_pos.setY(event->globalY());
        }

        float xoffset = event->globalX() - m_last_mouse_pos.x();
        float yoffset =  m_last_mouse_pos.y() - event->globalY();
        m_last_mouse_pos.setX(event->globalX());
        m_last_mouse_pos.setY(event->globalY());
        m_camera.processMouseMovement(xoffset, yoffset);
        update();
        //QOpenGLWidget::mouseMoveEvent(event);
   }
}

void OpenGLRenderer::wheelEvent(QWheelEvent* event) {
    int delta = event->delta();
    m_camera.processMouseScroll(delta);
    update();
}