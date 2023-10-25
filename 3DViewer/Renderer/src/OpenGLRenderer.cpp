#include <QEvent.h>
#include <QApplication>
#include <QtMath>

#include "../include/OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer(QWidget* parent) :
	QOpenGLWidget(parent),
	m_drawingMode(Mode::SOLID),
	m_camera(),
	m_scene()
{
	setFocusPolicy(parent->focusPolicy());
	setMouseTracking(true);
	//anti-alising
	QSurfaceFormat format;
	format.setSamples(4);
	setFormat(format);

	connect(this, &OpenGLRenderer::sceneUpdated,     &m_scene, &Scene::addObjectOnScene);
	connect(this, &OpenGLRenderer::sceneItemChanged, &m_scene, &Scene::handleSceneItemChanged);
	connect(this, &OpenGLRenderer::objectRemoved,    &m_scene, &Scene::removeCurrentObjSelection);
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
	m_shaderProgram->setUniformValue("objectColor", QVector3D(0.6f, 0.6f, 0.6f));
	m_shaderProgram->setUniformValue("ambientStrength", 0.2f);
	m_shaderProgram->setUniformValue("specularStrength", 0.5f);
	m_shaderProgram->setUniformValue("shininess", 128.0f);
}

void OpenGLRenderer::calculateFPS() {
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedTime = currentTime - m_lastFrameTime;
	m_lastFrameTime = currentTime;
	double fps = 1.0 / elapsedTime.count();
	emit framerateUpdated(QString::number(fps, 'f', 2));
}

void OpenGLRenderer::updateObjDetails(const std::shared_ptr<SceneObject>& obj)
{
	if (nullptr != obj) {
		emit nameUpdated(obj->getName());
		emit verticesUpdated(QString::number(obj->getNumberOfVertices()));
		emit facesUpdated(QString::number(obj->getNumberOfFaces()));
		emit edgesUpdated(QString::number(obj->getNumberOfEdges()));
		emit IdUpdated(QString::number(obj->getID()));
		emit widthUpdated(QString::number(obj->getWidth(), 'f', 2) + " cm");
		emit heightUpdated(QString::number(obj->getHeight(), 'f', 2) + " cm");
		emit lengthUpdated(QString::number(obj->getLength(), 'f', 2) + " cm");
	}
	else {
		emit nameUpdated("Unknown");
		emit verticesUpdated("0");
		emit facesUpdated("0");
		emit edgesUpdated("0");
		emit IdUpdated("N/A");
		emit widthUpdated("0.0");
		emit heightUpdated("0.0");
		emit lengthUpdated("0.0");
	}
}

void OpenGLRenderer::paintGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto& current_obj = std::move(m_scene.getCurrentObjSelection());
	if (nullptr != current_obj) {
		m_shaderProgram->bind();
		(m_drawingMode == Mode::SOLID) ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_MULTISAMPLE);
		if (!current_obj->isBuffersInited()) {
			current_obj->intializeBuffers(this);
			current_obj->setTranslationVec(-current_obj->getObjectCenter()); //move obj to center coordinate system
			m_camera.FitInWindow(current_obj->getBoundingBoxLength());
		}
		transform(current_obj);
		setUniforms();
		current_obj->draw(this);
		glDisable(GL_MULTISAMPLE);
	}
	updateObjDetails(current_obj);
	calculateFPS();
}

void OpenGLRenderer::initializeShaders() {
	QString current_source_filepath = QString::fromUtf8(__FILE__); // Full path to the current source file
	QDir source_file_dir = QFileInfo(current_source_filepath).dir(); // Directory of the source file
	QDir shaders_dir(source_file_dir.absoluteFilePath("../shaders")); // Path to shaders directory

	bool compile_result;
	m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	compile_result = m_vertexShader->compileSourceFile(shaders_dir.filePath("main_vert.glsl")); // Path to vertex shader file
	m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	compile_result = m_fragmentShader->compileSourceFile(shaders_dir.filePath("main_frag.glsl")); // Path to fragment shader file

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
		break;
	}
	update();
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
	update();
}

void OpenGLRenderer::wheelEvent(QWheelEvent* event) {
	int delta = event->delta();
	m_camera.processMouseScroll(delta);
	update();
}

void OpenGLRenderer::reset()
{
	const auto& current_obj = std::move(m_scene.getCurrentObjSelection());
	if (nullptr != current_obj) {
		current_obj->reset();
		m_camera.reset(current_obj->getBoundingBoxLength());
	}
	else {
		m_camera.reset();
	}
}

void OpenGLRenderer::processTranslation(QVector3D& delta)
{
	delta.setY(delta.y() * -1.0f);
	const auto& current_obj = std::move(m_scene.getCurrentObjSelection());
	if (nullptr != current_obj) {
		current_obj->setTranslationVec(current_obj->getTranslationVec() + delta * TRANSLATION_SPEED);
	}
}

void OpenGLRenderer::processRotation(QVector3D& delta)
{
	auto angle = qDegreesToRadians(delta.length()) * ANGLE_ROTATION_SCALE;
	auto axis = QVector3D(delta.y(), delta.x(), 0).normalized();
	auto rotation = QQuaternion::fromAxisAndAngle(axis, angle);
	const auto& current_obj = std::move(m_scene.getCurrentObjSelection());
	if (nullptr != current_obj) {
		current_obj->setRotationQuart(current_obj->getRotationQuart() * rotation);
	}
}