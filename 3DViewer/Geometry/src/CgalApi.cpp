#include "../include/CgalApi.h"
#include <QDebug>
#include <CGAL/property_map.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>

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

QVector3D CGAL_API::computeVertexNormal(const std::unique_ptr<Surface_mesh>& mesh, const CGAL::SM_Vertex_index& vertex)
{
    if (nullptr == mesh)
        return { 0, 0, 0 };
    auto vertex_normal = CGAL::Polygon_mesh_processing::compute_vertex_normal(vertex, *mesh);
    return QVector3D(
        vertex_normal.x(),
        vertex_normal.y(), 
        vertex_normal.z()
    );
}
