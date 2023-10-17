#pragma once

#include <QVector.h>
#include <QVector2D.h>
#include <QVector3D.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject.h>

#include "../../Geometry/include/CgalApi.h"

struct Vertex {

};

class OpenGLRenderer;

class SceneObject {
public:
	 SceneObject() = default;
	~SceneObject() = default;
	SceneObject(const SceneObject&) = delete;
	SceneObject(
		const QVector<QVector3D>& vertices,
		const QVector<QVector3D>& normals,
		const QVector<QVector2D>& textures = QVector<QVector2D>()
	);
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
	QVector<QVector3D> vertices;
private:
	QVector<QVector3D> m_normals;
	QVector<QVector2D> m_textures;
	
	unsigned int m_objID;
	static unsigned int m_idCounter;
	bool buffersInited;
};

template<typename T>
inline static std::shared_ptr<SceneObject> SceneObject::makeObject(const T& mesh)
{
	if (nullptr == mesh)
		return {};

	QVector<QVector3D> vertices;
	QVector<QVector3D> normals;
	QVector<QVector2D> textures; // add in future
	vertices.reserve(mesh->number_of_vertices());
	normals.reserve(mesh->number_of_vertices());

	for (const auto& face : mesh->faces()) {
		for (const auto& vertex : mesh->vertices_around_face(mesh->halfedge(face))) {
			const auto& vertex_point = mesh->point(vertex);
			vertices.push_back({
				static_cast<float>(CGAL::to_double(vertex_point.x())),
				static_cast<float>(CGAL::to_double(vertex_point.y())),
				static_cast<float>(CGAL::to_double(vertex_point.z()))
			});
			/*auto vertex_normal = CGAL_API::computeVertexNormal(mesh, vertex);
			normals.push_back({
				static_cast<float>(vertex_normal.x()),
				static_cast<float>(vertex_normal.y()),
				static_cast<float>(vertex_normal.z())
			});*/
		}
	}
	vertices.squeeze();
	normals.squeeze();

	return std::make_shared<SceneObject>(vertices, normals, textures);
}
