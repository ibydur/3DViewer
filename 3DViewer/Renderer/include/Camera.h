#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    Camera(const QVector3D& m_position = { 0.0f, 0.0f, 20.0f });

    // Getters for camera vectors and matrices
    QMatrix4x4 getViewMatrix();
    inline float getZoom() const { return this->m_zoom; }
    // Input handling functions
    void processMouseScroll(float yOffset);
    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void FitInWindow(float bbLength);
    void reset(float bbLength = 1.0f);
private:
    // Camera attributes
    QVector3D m_position;
    QVector3D m_front;
    QVector3D m_up;
    QVector3D m_right;
    QVector3D m_worldUp;

    // Euler angles
    float m_yaw;
    float m_pitch;

    // Camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;

    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;
    // Recalculate the camera vectors and matrices
    void m_updateCameraVectors();
};