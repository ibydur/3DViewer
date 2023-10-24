#pragma once
#include <QVector.h>
#include <QVector2D.h>
#include <QVector3D.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject.h>
#include <QElapsedTimer> 
#include <QQuaternion>
#include <QDir>

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
	SceneObject(const QString& filepath, const QString& name, const QVector<Vertex>& vertices,
		unsigned int vertices_count, unsigned int faces_count, unsigned int edges_count);

	template <typename T> inline static std::shared_ptr<SceneObject> makeObject(const QFileInfo& fileInfo, const T& mesh);

	void draw(OpenGLRenderer* renderer);
	void intializeBuffers(OpenGLRenderer* renderer);
	void release();
	void calculateDimensions();
	void reset();

	inline constexpr unsigned int getNumberOfVertices() const { return this->m_num_vertices; };
	inline constexpr unsigned int getNumberOfFaces()    const { return this->m_num_faces; };
	inline constexpr unsigned int getNumberOfEdges()    const { return this->m_num_edges; }
	inline constexpr float		  getWidth()			const { return this->m_width; };
	inline constexpr float		  getHeight()			const { return this->m_height; };
	inline constexpr float		  getLength()			const { return this->m_length; }
	inline QString				  getName()				const { return this->m_name; }
	inline QString				  getFilePath()			const { return this->m_filepath; }
	inline bool					  isActive()			const { return this->m_isActive; };
	inline unsigned int			  getID()				const { return this->m_objID; };
	inline bool					  isBuffersInited()		const { return this->m_buffersInited; };
	inline QVector3D			  getTranslationVec()	const { return this->m_translationVec; };
	inline QQuaternion			  getRotationQuart()	const { return this->m_rotationQuaternion; };

	inline void					  setBuffersInited(bool inited)			     { this->m_buffersInited = inited; };
	inline void					  setTranslationVec(const QVector3D& vec)    { this->m_translationVec = vec; };
	inline void					  setRotationQuart(const QQuaternion& quart) { this->m_rotationQuaternion = quart; };

	QVector3D getObjectCenter();

	QOpenGLBuffer vbo;
	QOpenGLVertexArrayObject vao;
	QVector<Vertex> vertices;

private:
	QString m_filepath;
	QString m_name;
	bool m_isActive;
	unsigned int m_objID;
	static unsigned int m_idCounter;
	bool m_buffersInited;
	// obj data
	unsigned int m_num_vertices;
	unsigned int m_num_faces;
	unsigned int m_num_edges;
	float m_width;
	float m_height;
	float m_length;
	QQuaternion m_rotationQuaternion;
	QVector3D m_translationVec;
};

template<typename T>
inline static std::shared_ptr<SceneObject> SceneObject::makeObject(const QFileInfo& fileInfo, const T& mesh)
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

	return std::make_shared<SceneObject>(
		fileInfo.absoluteFilePath(),
		fileInfo.baseName(),
		vertices,
		mesh->number_of_vertices(),
		mesh->number_of_faces(),
		mesh->number_of_edges()
	);
}
