#include <QDir>
#include <QEvent.h>
#include <QApplication>
#include <QtMath>

#include "../include/OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer(QWidget* parent) :
    QOpenGLWidget(parent),
    m_drawingMode(Mode::SOLID),
    m_camera(),
    m_scene(),
    m_lastFrame(0.0f), 
    m_deltaTime(0.0f)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    QSurfaceFormat format;
    format.setSamples(4);
    setFormat(format);
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
    obj.vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    obj.vbo.bind();

    obj.vbo.allocate(obj.vertices.constData(), obj.vertices.size() * sizeof(Vertex));

    m_shaderProgram->bind();
    //m_position
    m_shaderProgram->enableAttributeArray(0);
    m_shaderProgram->setAttributeArray(0, GL_FLOAT, 0, 3, sizeof(Vertex));
    //normal
    m_shaderProgram->enableAttributeArray(1);
    m_shaderProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));
    //texture
    m_shaderProgram->enableAttributeArray(2);
    m_shaderProgram->setAttributeBuffer(2, GL_FLOAT, sizeof(QVector3D) * 2, 2, sizeof(Vertex));

    obj.setBuffersInited(true);
}

void OpenGLRenderer::initializeGL() {
    m_timer.start();
    initializeOpenGLFunctions();
    initializeShaders();
    m_lastMousePos = QPoint(width() / 2.0f, height() / 2.0f);
}

void OpenGLRenderer::paintGL() {
    auto elapsed_time = static_cast<float>(m_timer.elapsed()) / 1000.0;
    m_deltaTime = elapsed_time - m_lastFrame;
    m_lastFrame = elapsed_time;
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Use the shader program
    m_shaderProgram->bind();
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getZoom(), static_cast<float>(width() / height()), 0.1f, 100.0f);
    m_view.setToIdentity();
    m_view = m_camera.getViewMatrix();
    m_model.setToIdentity();
    
    (m_drawingMode == Mode::SOLID) ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_MULTISAMPLE);
    for (const auto& obj : m_scene.getObjectsLst()) {
        if (!obj->isBuffersInited()) {
            obj->intializeBuffers(this);
            m_translationVec = -(obj->getObjectCenter());
        }
        m_model.translate(m_translationVec);
        m_model.rotate(m_rotationQuaternion);
        //vertex shader
        m_shaderProgram->setUniformValue("viewMatrix", m_view);
        m_shaderProgram->setUniformValue("projectionMatrix", m_projection);
        m_shaderProgram->setUniformValue("modelMatrix", m_model);
        //fragment shader
        m_shaderProgram->setUniformValue("lightDirectionFront", QVector3D(0.0f, 0.0f, -1.0f));
        m_shaderProgram->setUniformValue("lightDirectionBack", QVector3D(0.0f, 0.0f, 1.0f));
        m_shaderProgram->setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
        m_shaderProgram->setUniformValue("objectColor", QVector3D(0.6f, 0.6f, 0.6f));
        m_shaderProgram->setUniformValue("ambientStrength", 0.2f);
        m_shaderProgram->setUniformValue("specularStrength", 0.5f);
        m_shaderProgram->setUniformValue("shininess", 128.0f);

        obj->draw(this);
    }
    glDisable(GL_MULTISAMPLE);
}

void OpenGLRenderer::initializeShaders() {
    QString current_source_filepath = QString::fromUtf8(__FILE__); // Full path to the current source file
    QDir source_file_dir = QFileInfo(current_source_filepath).dir(); // Directory of the source file
    QDir shaders_dir(source_file_dir.absoluteFilePath("../shaders")); // Path to shaders directory

    bool compile_result;
    m_vertexShader   = new QOpenGLShader(QOpenGLShader::Vertex, this);
    compile_result    = m_vertexShader->compileSourceFile(shaders_dir.filePath("main_vert.glsl")); // Path to vertex shader file
    m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    compile_result    = m_fragmentShader->compileSourceFile(shaders_dir.filePath("main_frag.glsl")); // Path to fragment shader file

    // Create and link shader program
    m_shaderProgram = new QOpenGLShaderProgram(this);
    m_shaderProgram->addShader(m_vertexShader);
    m_shaderProgram->addShader(m_fragmentShader);
    m_shaderProgram->link(); 

    delete m_vertexShader;
    delete m_fragmentShader;
}

void OpenGLRenderer::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        QApplication::quit();
    case Qt::Key_W:
        m_camera.processKeyboard(CameraMovement::FORWARD, m_deltaTime);
        break;
    case Qt::Key_S:
        m_camera.processKeyboard(CameraMovement::BACKWARD, m_deltaTime);
        break;
    case Qt::Key_A:
        m_camera.processKeyboard(CameraMovement::LEFT, m_deltaTime);
        break;
    case Qt::Key_D:
        m_camera.processKeyboard(CameraMovement::RIGHT, m_deltaTime);
        break;
    case Qt::Key_R:
        m_camera.reset();
        this->reset();
        break;
    case Qt::Key_C:
        m_drawingMode = (m_drawingMode == Mode::SOLID) ? Mode::WIREFRAME : Mode::SOLID;
        break;
    }
    update();
}

void OpenGLRenderer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() &
        Qt::LeftButton  &
        Qt::RightButton &
        (Qt::LeftButton | Qt::RightButton)) {
        m_lastMousePos = event->globalPos();
    }
}

void OpenGLRenderer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() & 
        Qt::LeftButton  & 
        Qt::RightButton & 
        (Qt::LeftButton | Qt::RightButton)) {
        m_lastMousePos = event->globalPos();
    }
}

void OpenGLRenderer::mouseMoveEvent(QMouseEvent* event) {
    auto delta_pos = event->globalPos() - m_lastMousePos;
	QVector3D delta_vec = { 
        static_cast<float>(delta_pos.x()),
        static_cast<float>(delta_pos.y()),
        0.0f
    };
	if (event->buttons() == (Qt::LeftButton | Qt::RightButton)) {
        processTranslation(delta_vec);
	}
	else if (event->buttons() & Qt::RightButton) {
		m_camera.processMouseMovement(delta_vec.x(), -delta_vec.y());
	}
	else if (event->buttons() & Qt::LeftButton) {
        processRotation(delta_vec);
	}
	m_lastMousePos = event->globalPos();
	update();
}

void OpenGLRenderer::wheelEvent(QWheelEvent* event) {
    int delta = event->delta();
    m_camera.processMouseScroll(delta);
    update();
}

void OpenGLRenderer::reset()
{
    m_rotationQuaternion = QQuaternion::fromAxisAndAngle({ 0, 0, 0 }, 0);
    m_translationVec = { 0.0f, 0.0f, 0.0f };
    update();
}

void OpenGLRenderer::processTranslation(QVector3D& delta)
{
    delta.setY(delta.y() * -1.0f);
    m_translationVec += delta * TRANSLATION_SPEED;
}

void OpenGLRenderer::processRotation(QVector3D& delta)
{
    auto angle = qDegreesToRadians(delta.length()) * ANGLE_ROTATION_SCALE;
    QVector3D axis = QVector3D(delta.y(), delta.x(), 0).normalized();
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(axis, angle);
    m_rotationQuaternion *= rotation;
}