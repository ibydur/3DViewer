#include <qmath.h>

#include "../include/Camera.h"

Camera::Camera(const QVector3D& m_position)
    : m_position(m_position), m_front(QVector3D(0.0f, 0.0f, -1.0f)),
    m_up(QVector3D(0.0f, 1.0f, 0.0f)), m_right(QVector3D(1.0f, 0.0f, 0.0f)),
    m_worldUp(QVector3D(0.0f, 1.0f, 0.0f)), m_yaw(-90.0f), m_pitch(0.0f),
    m_movementSpeed(3.0f), m_mouseSensitivity(0.05f), m_zoom(45.0f) {
    
    m_updateCameraVectors();
}

QMatrix4x4 Camera::getViewMatrix()
{
    m_view.setToIdentity();
    m_view.lookAt(m_position, m_position + m_front, m_up); 
    return this->m_view;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = m_movementSpeed * 0.05;
    if (direction == CameraMovement::FORWARD)
        m_position += m_front * velocity;
    if (direction == CameraMovement::BACKWARD)
        m_position -= m_front * velocity;
    if (direction == CameraMovement::LEFT)
        m_position -= m_right * velocity;
    if (direction == CameraMovement::RIGHT)
        m_position += m_right * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    // Clamp m_pitch to avoid flipping
    m_pitch = qBound(-89.0f, m_pitch, 89.0f);

    m_updateCameraVectors();
}

void Camera::reset()
{
    this->m_position = QVector3D(0.0f, 0.0f, 20.0f);
    this->m_zoom = 45.0f;
    this->m_yaw = -90.0f;
    this->m_pitch = 0.0f;
    m_updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset) {
    if (m_zoom >= 1.0f && m_zoom <= 120.0f)
        m_zoom -= yOffset * 0.05;
    if (m_zoom <= 1.0f)
        m_zoom = 1.0f;
    if (m_zoom >= 120.0f)
        m_zoom = 120.0f;
}

void Camera::m_updateCameraVectors() {
    QVector3D newm_front = { 0, 0, 0 };
    newm_front.setX(qCos(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    newm_front.setY(qSin(qDegreesToRadians(m_pitch)));
    newm_front.setZ(qSin(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    m_front = newm_front.normalized();

    m_right = QVector3D::crossProduct(m_front, m_worldUp).normalized();
    m_up = QVector3D::crossProduct(m_right, m_front).normalized();
}