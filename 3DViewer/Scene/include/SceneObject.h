#pragma once
#include <QVector.h>
#include <QVector2D.h>
#include <QVector3D.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject.h>
#include <QElapsedTimer> 
#include <QQuaternion>
#include <QDir>

#include "CgalApi.h"

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
	SceneObject(const QString& filepath, const QString& name, const QVector<Vertex>& vertices,
		unsigned int vertices_count, unsigned int faces_count, unsigned int edges_count);

	template <typename T> inline static std::shared_ptr<SceneObject> makeObject(const QFileInfo& fileInfo, const T& mesh);

	void draw(OpenGLRenderer* renderer);
	void intializeBuffers(OpenGLRenderer* renderer);
	void release();
	void calculateBoundingBox();

	void reset();

	inline constexpr unsigned int getNumberOfVertices() const { return this->m_num_vertices; };
	inline constexpr unsigned int getNumberOfFaces()    const { return this->m_num_faces; };
	inline constexpr unsigned int getNumberOfEdges()    const { return this->m_num_edges; }
	inline constexpr unsigned int getID()				const { return this->m_objID; };
	inline constexpr float		  getWidth()			const { return this->m_width; };
	inline constexpr float		  getHeight()			const { return this->m_height; };
	inline constexpr float		  getLength()			const { return this->m_length; }
	inline constexpr float		  getBoundingBoxLength()const { return this->m_boundingBoxLength; }
	inline QString				  getName()				const { return this->m_name; }
	inline QString				  getFilePath()			const { return this->m_filepath; }
	inline int					  isVisible()			const { return this->m_isVisible; };
	inline bool					  isBuffersInited()		const { return this->m_buffersInited; };
	inline QVector3D			  getObjectCenter()		const { return this->m_center; }
	inline QVector3D			  getTranslationVec()	const { return this->m_translationVec; };
	inline QQuaternion			  getRotationQuart()	const { return this->m_rotationQuaternion; };

	inline void					  setBuffersInited(bool inited) { this->m_buffersInited = inited; };
	inline void					  setTranslationVec(const QVector3D& vec) { this->m_translationVec = vec; };
	inline void					  setRotationQuart(const QQuaternion& quart) { this->m_rotationQuaternion = quart; };
	inline void					  setVisible(int state) { this->m_isVisible = state; };

	QOpenGLBuffer vbo;
	QOpenGLVertexArrayObject vao;
	QVector<Vertex> vertices;

private:
	static unsigned int m_idCounter;
	bool m_buffersInited;
	// obj data
	QString m_filepath;
	QString m_name;
	unsigned int m_num_vertices;
	unsigned int m_num_faces;
	unsigned int m_num_edges;
	unsigned int m_objID;
	float m_width;
	float m_height;
	float m_length;
	float m_boundingBoxLength;
	int m_isVisible;
	QQuaternion m_rotationQuaternion;
	QVector3D m_translationVec;
	QVector3D m_center;
};

template<typename T>
inline static std::shared_ptr<SceneObject> SceneObject::makeObject(const QFileInfo& fileInfo, const T& mesh)
{
	if (nullptr == mesh) {
		return nullptr;
	}
	//try block only for cgal api exceptions
	try {
		QElapsedTimer timer;
		timer.start();
		qDebug() << "Message: scene object creation has been started";
		QVector<Vertex> vertices;
		for (const auto& face : mesh->faces()) {
			for (const auto& vertex : mesh->vertices_around_face(mesh->halfedge(face))) {
				Vertex custom_vertex;
				const auto& vertex_point = mesh->point(vertex);
				custom_vertex.position = {
					static_cast<float>(CGAL::to_double(vertex_point.x())),
					static_cast<float>(CGAL::to_double(vertex_point.y())),
					static_cast<float>(CGAL::to_double(vertex_point.z()))
				};
				custom_vertex.normal = CGAL_API::computeVertexNormal(mesh, vertex);
				custom_vertex.texture = { 0, 0 };
				vertices.push_back(custom_vertex);
			}
		}
		qDebug() << "Message: scene object creation took" << 
			static_cast<double>(timer.nsecsElapsed()) / 1000000000.0 << "sec to execute";

		return std::make_shared<SceneObject>(
			fileInfo.absoluteFilePath(),
			fileInfo.baseName(),
			vertices,
			mesh->number_of_vertices(),
			mesh->number_of_faces(),
			mesh->number_of_edges()
		);
	}
	catch (const std::exception& exp)
	{
		qCritical() << exp.what();
		return nullptr;
	}
}
