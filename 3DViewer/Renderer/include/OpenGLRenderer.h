#pragma once

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFunctions_3_3_Core>
#include <QElapsedTimer>

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

	QPoint m_last_mouse_pos;

	QMatrix4x4 m_projection;
	QMatrix4x4 m_view;
	QMatrix4x4 m_model;

	QOpenGLShader* m_vertex_shader;
	QOpenGLShader* m_fragment_shader;
	QOpenGLShaderProgram* m_shader_program;

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
	bool mFirstTimeHandled = false;
	QElapsedTimer timer;

	Mode drawingMode;

protected:
	
};