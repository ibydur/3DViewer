#include "../include/SceneObject.h"
#include "../../Renderer/include/OpenGLRenderer.h"

unsigned int SceneObject::m_idCounter = 0;

SceneObject::SceneObject(const QVector<QVector3D>& vertices, const QVector<QVector3D>& normals, const QVector<QVector2D>& textures) : 
	vertices(vertices), m_normals(normals), m_textures(textures),
	m_objID(++m_idCounter), buffersInited(false)
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
		objectCenter += vertex;
	}
	objectCenter /= vertices.size();
	return objectCenter;
}
