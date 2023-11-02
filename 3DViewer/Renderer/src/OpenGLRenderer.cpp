#include <QEvent.h>
#include <QApplication>
#include <QtMath>

#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer(QWidget* parent, const Scene& scene) :
	QOpenGLWidget(parent),
	m_drawingMode(Mode::SOLID),
	m_camera(),
	m_scene(scene)
{
	setFocusPolicy(parent->focusPolicy());
	setMouseTracking(true);
	//anti-alising
	QSurfaceFormat format;
	format.setSamples(4);
	setFormat(format); 
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

void OpenGLRenderer::updateCamera(float bblength) 
{
    m_camera.reset(bblength);
}

void OpenGLRenderer::redraw(void)
{
	this->update();
}

void OpenGLRenderer::initializeGL() {
	m_timer.start();
	initializeOpenGLFunctions();
	initializeShaders();
	m_lastMousePos = QPoint(width() / 2.0f, height() / 2.0f);
}

void OpenGLRenderer::transform(const std::shared_ptr<SceneObject>& obj)
{
	m_projection.setToIdentity();
	m_projection.perspective(m_camera.getZoom(), static_cast<float>(width() / height()), 0.1f, 10000.0f);
	m_view.setToIdentity();
	m_view = m_camera.getViewMatrix();
	m_model.setToIdentity();
	m_model.translate(obj->getTranslationVec());
	m_model.rotate(obj->getRotationQuart());
}

void OpenGLRenderer::setUniforms()
{
	//vertex shader
	m_shaderProgram->setUniformValue("viewMatrix", m_view);
	m_shaderProgram->setUniformValue("projectionMatrix", m_projection);
	m_shaderProgram->setUniformValue("modelMatrix", m_model);
	//fragment shader
	m_shaderProgram->setUniformValue("lightDirectionFront", QVector3D(0.0f, 0.0f, -1.0f));
	m_shaderProgram->setUniformValue("lightDirectionBack", QVector3D(0.0f, 0.0f, 1.0f));
	m_shaderProgram->setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
	//material
	auto current_material = m_scene.getCurrentMaterial();
	m_shaderProgram->setUniformValue("objectColor", current_material.objectColor);
	m_shaderProgram->setUniformValue("ambientStrength", current_material.ambientStrength);
	m_shaderProgram->setUniformValue("specularStrength", current_material.specularStrength);
	m_shaderProgram->setUniformValue("shininess", current_material.shininess);
}

void OpenGLRenderer::calculateFPS() {
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedTime = currentTime - m_lastFrameTime;
	m_lastFrameTime = currentTime;
	double fps = 1.0 / elapsedTime.count();
	emit framerateUpdated(QString::number(fps, 'f', 2));
}

void OpenGLRenderer::paintGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto& current_obj = std::move(m_scene.getCurrentObjSelection());
	if (nullptr != current_obj && Qt::CheckState::Checked == current_obj->isVisible()) {
		m_shaderProgram->bind();
		(m_drawingMode == Mode::SOLID) ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_MULTISAMPLE);
		if (!current_obj->isBuffersInited()) {
			current_obj->intializeBuffers(this);
			current_obj->setTranslationVec(-current_obj->getObjectCenter()); //move obj to center coordinate system
		}
		transform(current_obj);
		setUniforms();
		current_obj->draw(this);
		glDisable(GL_MULTISAMPLE);
	}
	m_scene.updateObjDetails(current_obj);
	calculateFPS();
}

void OpenGLRenderer::initializeShaders() 
{
	m_shaderProgram = new QOpenGLShaderProgram(this);
	if (!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/main_vert.glsl")) {
		qCritical() << "Critical: error while loading vertex shader.";
	}
	if (!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/main_frag.glsl")) {
		qCritical() << "Critical: error while loading fragment shader.";
	}
}

void OpenGLRenderer::keyPressEvent(QKeyEvent* event) {
	switch (event->key()) {
	case Qt::Key_W:
		m_camera.processKeyboard(CameraMovement::FORWARD, 0);
		break;
	case Qt::Key_S:
		m_camera.processKeyboard(CameraMovement::BACKWARD, 0);
		break;
	case Qt::Key_A:
		m_camera.processKeyboard(CameraMovement::LEFT, 0);
		break;
	case Qt::Key_D:
		m_camera.processKeyboard(CameraMovement::RIGHT, 0);
		break;
	case Qt::Key_R:
		reset();
		break;
	case Qt::Key_C:
		m_drawingMode = (m_drawingMode == Mode::SOLID) ? Mode::WIREFRAME : Mode::SOLID;
		emit drawingModeChanged(m_drawingMode == Mode::SOLID ? "solid" : "wireframe");
		break;
	}
	redraw();
}

void OpenGLRenderer::mousePressEvent(QMouseEvent* event)
{
	if (event->button() &
		Qt::LeftButton &
		Qt::RightButton &
		(Qt::LeftButton | Qt::RightButton)) {
		m_lastMousePos = event->globalPos();
	}
}

void OpenGLRenderer::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() &
		Qt::LeftButton &
		Qt::RightButton &
		(Qt::LeftButton | Qt::RightButton)) {
		m_lastMousePos = event->globalPos();
	}
}

void OpenGLRenderer::mouseMoveEvent(QMouseEvent* event)
{
	emit mouseMoved(QStringLiteral("x = %1, y = %2").arg(event->pos().x()).arg(event->pos().y()));
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
	redraw();
}

void OpenGLRenderer::wheelEvent(QWheelEvent* event) {
	int delta = event->delta();
	m_camera.processMouseScroll(delta);
	redraw();
}

void OpenGLRenderer::reset()
{
	const auto& current_obj = std::move(m_scene.getCurrentObjSelection());
	if (nullptr != current_obj) {
		current_obj->reset();
        updateCamera(current_obj->getBoundingBoxLength());
	}
}

void OpenGLRenderer::processTranslation(QVector3D& delta)
{
	delta.setY(delta.y() * -1.0f);
	const auto& current_obj = std::move(m_scene.getCurrentObjSelection());
	if (nullptr != current_obj) {
		current_obj->setTranslationVec(current_obj->getTranslationVec() + delta * m_scene.TRANSLATION_SPEED);
	}
}

void OpenGLRenderer::processRotation(QVector3D& delta)
{
	auto angle = qDegreesToRadians(delta.length()) * m_scene.ANGLE_ROTATION_SCALE;
	auto axis = QVector3D(delta.y(), delta.x(), 0).normalized();
	auto rotation = QQuaternion::fromAxisAndAngle(axis, angle);
	const auto& current_obj = std::move(m_scene.getCurrentObjSelection());
	if (nullptr != current_obj) {
		current_obj->setRotationQuart(current_obj->getRotationQuart() * rotation);
	}
}