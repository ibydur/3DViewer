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
	static constexpr auto TRANSLATION_SPEED = 0.02f;
	static constexpr auto ANGLE_ROTATION_SCALE = 7.0f;
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

signals:
	void mouseMoved(QString);
	void framerateUpdated(QString);
	void sceneStatusUpdated(QString);
	void verticesUpdated(QString);
	void edgesUpdated(QString);
	void facesUpdated(QString);
	void IdUpdated(QString);
	void widthUpdated(QString);
	void lengthUpdated(QString);
	void heightUpdated(QString);
	void nameUpdated(QString);
	void sceneUpdated(const std::shared_ptr<SceneObject>&);
	void sceneItemChanged(QListWidgetItem*, QListWidgetItem*);
	void objectRemoved();

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
	void updateObjDetails(const std::shared_ptr<SceneObject>& obj);
	void reset();
	void processTranslation(QVector3D& delta);
	void processRotation(QVector3D& delta);

	Camera m_camera;
	Scene  m_scene;

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