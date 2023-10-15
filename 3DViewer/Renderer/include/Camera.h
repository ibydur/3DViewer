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
    Camera(const QVector3D& position = { 0.0f, 0.0f, 20.0f });

    // Getters for camera vectors and matrices
    QMatrix4x4 getViewMatrix();
    inline float getZoom() const { return this->zoom; }
    // Input handling functions
    void processMouseScroll(float yOffset);
    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void reset();
private:
    // Camera attributes
    QVector3D position;
    QVector3D front;
    QVector3D up;
    QVector3D right;
    QVector3D worldUp;

    // Euler angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    QMatrix4x4 view;
    QMatrix4x4 projection;
    // Recalculate the camera vectors and matrices
    void updateCameraVectors();
};