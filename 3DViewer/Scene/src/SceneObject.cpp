#include "../include/SceneObject.h"
#include "../../Renderer/include/OpenGLRenderer.h"

unsigned int SceneObject::m_idCounter = 0;

SceneObject::SceneObject(const QVector<Vertex>& vertices) :
	vertices(vertices), m_objID(++m_idCounter), buffersInited(false)
{
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
