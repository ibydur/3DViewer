#pragma once

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLFunctions_3_3_Core>
#include <QElapsedTimer>

#include "../include/Camera.h"

class OpenGLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    //Q_OBJECT
public:
    OpenGLRenderer(QWidget* parent) : 
        QOpenGLWidget(parent),
        camera(std::make_unique<Camera>())
    {
        setFocusPolicy(Qt::StrongFocus); // To receive keyboard focus
        setMouseTracking(true); // Enable mouse tracking
    }

    ~OpenGLRenderer();
protected:
    void initializeGL() override;
    void paintGL() override;

    void initializeShaders();
    void setupVboAndVao();
private:
    std::unique_ptr<Camera> camera;
    QPoint m_last_mouse_pos;
    QOpenGLShader* m_vertex_shader;
    QOpenGLShader* m_fragment_shader;
    QOpenGLShaderProgram* m_shader_program;
    GLuint m_vbo, m_vao;
    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;
    QMatrix4x4 m_model;

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    bool mFirstTimeHandled = false;
    QElapsedTimer timer;
  
protected:
signals:
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};