#pragma once

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFunctions_3_3_Core>
#include <QElapsedTimer>

#include "../include/Camera.h"

class OpenGLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {

public:
	enum class Mode {
		WIREFRAME,
		SOLID
	};

	OpenGLRenderer(QWidget* parent) :
		QOpenGLWidget(parent),
		camera(std::make_unique<Camera>())
	{
		setFocusPolicy(Qt::StrongFocus); // To receive keyboard focus
		setMouseTracking(true); // Enable mouse tracking
	}
	//void setupVboAndVao();
	inline void setMeshVertices(const  QVector<QVector3D>& vertices) { m_vertices = vertices; }
	QVector3D getObjectCenter();
	~OpenGLRenderer();

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
	std::unique_ptr<Camera> camera;
	QPoint m_last_mouse_pos;

	QMatrix4x4 m_projection;
	QMatrix4x4 m_view;
	QMatrix4x4 m_model;

	QOpenGLShader* m_vertex_shader;
	QOpenGLShader* m_fragment_shader;
	QOpenGLShaderProgram* m_shader_program;
	GLuint m_vbo, m_vao;

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
	bool mFirstTimeHandled = false;
	QElapsedTimer timer;
	QVector<QVector3D> m_vertices;

	Mode drawingMode = Mode::SOLID;

protected:
	
};