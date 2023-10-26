#include "../include/SceneObject.h"
#include "../../Renderer/include/OpenGLRenderer.h"

unsigned int SceneObject::m_idCounter = 0;

SceneObject::SceneObject(
    const QString& filepath, const QString& name, const QVector<Vertex>& vertices,
	unsigned int vertices_count, unsigned int faces_count, unsigned int edges_count
) :
	m_filepath(filepath), m_name(name), m_num_vertices(vertices_count), 
    m_num_faces(faces_count), m_num_edges(edges_count), vertices(vertices), 
    m_objID(++m_idCounter),	m_buffersInited(false), m_isVisible(Qt::CheckState::Checked)
{
    calculateBoundingBox();
}

void SceneObject::draw(OpenGLRenderer* renderer)
{
    renderer->drawObject(*this);
}

void SceneObject::intializeBuffers(OpenGLRenderer* renderer)
{
    renderer->initObjectBuffers(*this);
}

void SceneObject::release()
{
    vbo.destroy();
    vao.destroy();
}

void SceneObject::calculateBoundingBox()
{
    QVector3D minBounds = QVector3D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    QVector3D maxBounds = QVector3D(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    for (const auto& vertex : vertices) {
        minBounds.setX(std::min(minBounds.x(), vertex.position.x()));
        minBounds.setY(std::min(minBounds.y(), vertex.position.y()));
        minBounds.setZ(std::min(minBounds.z(), vertex.position.z()));

        maxBounds.setX(std::max(maxBounds.x(), vertex.position.x()));
        maxBounds.setY(std::max(maxBounds.y(), vertex.position.y()));
        maxBounds.setZ(std::max(maxBounds.z(), vertex.position.z()));
    }
    //converting to cm
    m_length            = (maxBounds.x() - minBounds.x()) * 100.0f;
    m_width             = (maxBounds.y() - minBounds.y()) * 100.0f;
    m_height            = (maxBounds.z() - minBounds.z()) * 100.0f;
    m_center            = (minBounds + maxBounds) * 0.5;
    m_boundingBoxLength = (maxBounds - minBounds).length();
}

void SceneObject::reset()
{
    m_rotationQuaternion = QQuaternion::fromAxisAndAngle({ 0, 0, 0 }, 0);
    m_translationVec = -getObjectCenter();
}
