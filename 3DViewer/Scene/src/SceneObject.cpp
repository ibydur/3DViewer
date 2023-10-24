#include "../include/SceneObject.h"
#include "../../Renderer/include/OpenGLRenderer.h"

unsigned int SceneObject::m_idCounter = 0;

SceneObject::SceneObject(
    const QString& filepath, const QString& name, const QVector<Vertex>& vertices,
	unsigned int vertices_count, unsigned int faces_count, unsigned int edges_count
) :
	m_filepath(filepath), m_name(name), m_num_vertices(vertices_count), 
    m_num_faces(faces_count), m_num_edges(edges_count), vertices(vertices), 
    m_objID(++m_idCounter),	m_buffersInited(false), m_isActive(true)
{
	calculateDimensions();
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

QVector3D SceneObject::getObjectCenter()
{
	QVector3D objectCenter;
	for (const auto& vertex : vertices) {
		objectCenter += vertex.position;
	}
	objectCenter /= vertices.size();
	return objectCenter;
}

void SceneObject::calculateDimensions()
{
    QVector3D minCoords(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    QVector3D maxCoords(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    for (const Vertex& vertex : vertices) {
        minCoords.setX(std::min(minCoords.x(), vertex.position.x()));
        minCoords.setY(std::min(minCoords.y(), vertex.position.y()));
        minCoords.setZ(std::min(minCoords.z(), vertex.position.z()));

        maxCoords.setX(std::max(maxCoords.x(), vertex.position.x()));
        maxCoords.setY(std::max(maxCoords.y(), vertex.position.y()));
        maxCoords.setZ(std::max(maxCoords.z(), vertex.position.z()));
    }
    m_length = (maxCoords.x() - minCoords.x()) * 100.0f;
    m_width  = (maxCoords.y() - minCoords.y()) * 100.0f;
    m_height = (maxCoords.z() - minCoords.z()) * 100.0f;
}
