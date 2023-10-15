#include "../include/Camera.h"
#include <qmath.h>

Camera::Camera(const QVector3D& position)
    : position(position), front(QVector3D(0.0f, 0.0f, -1.0f)),
    up(QVector3D(0.0f, 1.0f, 0.0f)), right(QVector3D(1.0f, 0.0f, 0.0f)),
    worldUp(QVector3D(0.0f, 1.0f, 0.0f)), yaw(-90.0f), pitch(0.0f),
    movementSpeed(3.0f), mouseSensitivity(0.05f), zoom(45.0f) {
    
    updateCameraVectors();
}

QMatrix4x4 Camera::getViewMatrix()
{
    view.setToIdentity();
    view.lookAt(position, position + front, up); 
    return this->view;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = movementSpeed * 0.05;
    if (direction == CameraMovement::FORWARD)
        position += front * velocity;
    if (direction == CameraMovement::BACKWARD)
        position -= front * velocity;
    if (direction == CameraMovement::LEFT)
        position -= right * velocity;
    if (direction == CameraMovement::RIGHT)
        position += right * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Clamp pitch to avoid flipping
    pitch = qBound(-89.0f, pitch, 89.0f);

    updateCameraVectors();
}

void Camera::reset()
{
    this->position = QVector3D(0.0f, 0.0f, 5.0f);
    this->zoom = 45.0f;
    this->yaw = -90.0f;
    this->pitch = 0.0f;
    updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset) {
    if (zoom >= 1.0f && zoom <= 120.0f)
        zoom -= yOffset * 0.05;
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 120.0f)
        zoom = 120.0f;
}

void Camera::updateCameraVectors() {
    QVector3D newFront = { 0, 0, 0 };
    newFront.setX(qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    newFront.setY(qSin(qDegreesToRadians(pitch)));
    newFront.setZ(qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    front = newFront.normalized();

    right = QVector3D::crossProduct(front, worldUp).normalized();
    up = QVector3D::crossProduct(right, front).normalized();
}