#pragma once
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFunctions_3_3_Core>
#include <QElapsedTimer>
#include <QSurfaceFormat>

#include "../include/Camera.h"
#include "../../Scene/include/Scene.h"

class OpenGLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {

public:
	enum class Mode {
		WIREFRAME,
		SOLID
	};

	OpenGLRenderer(QWidget* parent = nullptr);
	~OpenGLRenderer();

	void addObject(const std::shared_ptr<SceneObject>& obj);
	void drawObject(SceneObject& obj);
	void initObjectBuffers(SceneObject& obj);

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
	Camera m_camera;
	Scene  m_scene;

	QPoint m_lastMousePos;

	QMatrix4x4 m_projection;
	QMatrix4x4 m_view;
	QMatrix4x4 m_model;

	QOpenGLShader* m_vertexShader;
	QOpenGLShader* m_fragmentShader;
	QOpenGLShaderProgram* m_shaderProgram;

	float m_lastFrame;
	float m_deltaTime;
	bool m_firstTimeHandled;
	QElapsedTimer m_timer;

	Mode m_drawingMode;
};