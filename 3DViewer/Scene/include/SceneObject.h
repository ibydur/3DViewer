#pragma once
#include <QVector.h>
#include <QVector2D.h>
#include <QVector3D.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject.h>
#include <QElapsedTimer> 

#include "../../Geometry/include/CgalApi.h"

struct Vertex {
	QVector3D position;
	QVector3D normal;
	QVector2D texture;
};

class OpenGLRenderer;

class SceneObject {
public:
	 SceneObject() = default;
	~SceneObject() = default;
	SceneObject(const SceneObject&) = delete;
	SceneObject(const QVector<Vertex>& vertices);
	template <typename T> inline static std::shared_ptr<SceneObject> makeObject(const T& mesh);
	inline unsigned int getID() const { return this->m_objID; };

	void draw(OpenGLRenderer* renderer);
	void intializeBuffers(OpenGLRenderer* renderer);
	void release();

	inline bool isBuffersInited() const { return this->buffersInited; };
	inline void setBuffersInited(bool inited) { this->buffersInited = inited; };

	QVector3D getObjectCenter();

	QOpenGLBuffer vbo;
	QOpenGLVertexArrayObject vao;
	QVector<Vertex> vertices;
private:
	
	unsigned int m_objID;
	static unsigned int m_idCounter;
	bool buffersInited;
};

template<typename T>
inline static std::shared_ptr<SceneObject> SceneObject::makeObject(const T& mesh)
{
	if (nullptr == mesh) {
		return {};
	}

	QElapsedTimer timer;
	timer.start();
	qDebug() << "Function makeObject started";
	QVector<Vertex> vertices;
	for (const auto& face : mesh->faces()) {
		for (const auto& vertex : mesh->vertices_around_face(mesh->halfedge(face))) {
			Vertex custom_vertex;
			const auto& vertex_point = mesh->point(vertex);
			custom_vertex.position = {
				vertex_point.x(),
				vertex_point.y(),
				vertex_point.z()
			};
			custom_vertex.normal = CGAL_API::computeVertexNormal(mesh, vertex);
			custom_vertex.texture = { 0, 0 };
			vertices.push_back(custom_vertex);
		}
	}
	qint64 elapsed = timer.nsecsElapsed();
	double elapsedSeconds = static_cast<double>(elapsed) / 1000000000.0; // Convert nanoseconds to seconds
	qDebug() << "Function took" << elapsedSeconds << "seconds to execute";
	return std::make_shared<SceneObject>(vertices);
}
