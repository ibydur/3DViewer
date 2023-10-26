#pragma once
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFunctions_3_3_Core>
#include <QElapsedTimer>
#include <QSurfaceFormat>
#include <QListWidget>

#include "../include/Camera.h"
#include "../../Scene/include/Scene.h"


class OpenGLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
	Q_OBJECT
public:
	enum class Mode {
		WIREFRAME,
		SOLID
	};
	OpenGLRenderer(QWidget* parent = nullptr, const Scene& scene = Scene());
	~OpenGLRenderer();

	void drawObject(SceneObject& obj);
	void initObjectBuffers(SceneObject& obj);

signals:
	void mouseMoved(QString);
	void framerateUpdated(QString);

protected:
	void initializeGL() override;
	void paintGL() override;
	void initializeShaders();

	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	void transform(const std::shared_ptr<SceneObject>& obj);
	void setUniforms();
	void calculateFPS();
	void reset();
	void processTranslation(QVector3D& delta);
	void processRotation(QVector3D& delta);

	Camera m_camera;
	const Scene&  m_scene;

	QPointF m_lastMousePos;

	QMatrix4x4 m_projection;
	QMatrix4x4 m_view;
	QMatrix4x4 m_model;

	QOpenGLShader* m_vertexShader;
	QOpenGLShader* m_fragmentShader;
	QOpenGLShaderProgram* m_shaderProgram;

	QElapsedTimer m_timer;
	Mode m_drawingMode;
	std::chrono::high_resolution_clock::time_point m_lastFrameTime;
};