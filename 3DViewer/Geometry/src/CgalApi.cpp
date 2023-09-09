#include "../include/CgalApi.h"
#include <QDebug>
#include <CGAL/property_map.h>

std::unique_ptr<Surface_mesh> CGAL_API::constructMeshFromObj(const std::string& file_path)
{
    auto mesh = std::make_unique<Surface_mesh>();
    std::ifstream input(file_path);
    if (!input) {
        qWarning() << "CGAL API error: Cannot open input file.";
        return nullptr;
    }
    if (!CGAL::IO::read_OBJ(input, *mesh)) {
        qWarning() << "CGAL API error: Failed to read OBJ file.";
        return nullptr;
    }
    input.close();
    return mesh;
}

QVector<QVector3D> CGAL_API::loadMeshToVector(const std::unique_ptr<Surface_mesh>& mesh)
{
    if (nullptr == mesh) 
        return {};

    QVector<QVector3D> vertices;
    vertices.reserve(mesh->number_of_vertices());

    for (const auto& face : mesh->faces()) {
        for (const auto& vertex : mesh->vertices_around_face(mesh->halfedge(face))) {
            const auto& point = mesh->point(vertex);
            vertices.push_back({
                static_cast<float>(CGAL::to_double(point.x())),
                static_cast<float>(CGAL::to_double(point.y())),
                static_cast<float>(CGAL::to_double(point.z()))
                });
        }
    }
    vertices.shrink_to_fit();
    return vertices;
}
