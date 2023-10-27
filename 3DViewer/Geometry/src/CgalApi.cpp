#include "../include/CgalApi.h"

#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>

#include <QDebug>
#include <QElapsedTimer>

std::unique_ptr<Surface_mesh> CGAL_API::constructMeshFromObj(const std::string& file_path)
{
    //try block only for cgal exceptions
    try {
        auto mesh = std::make_unique<Surface_mesh>();
        std::ifstream input(file_path);
        if (!input) {
            qCritical() << "Critical CGAL API: cannot open input file.";
            return nullptr;
        }
        QElapsedTimer timer;
        timer.start();
        qDebug() << "Message: obj reading has been started: " << file_path.c_str();
        if (!CGAL::IO::read_OBJ(input, *mesh)) {
            qCritical() << "Critical: CGAL API failed to read OBJ file.";
            return nullptr;
        }
        qDebug() << "Message: obj reading has been ended and took " << 
            static_cast<double>(timer.nsecsElapsed()) / 1000000000.0 << " sec: " 
            << file_path.c_str();
        input.close();

        if (!checkConstructedMesh(mesh, file_path)) {
            return nullptr;
        }
        //triangulate mesh because we dont know how its constructed in obj
        CGAL::Polygon_mesh_processing::triangulate_faces(*mesh);
        return mesh;
    }
    catch (const std::exception& exp)
    {
        qCritical() << exp.what();
        return nullptr;
    }
}

QVector3D CGAL_API::computeVertexNormal(const std::unique_ptr<Surface_mesh>& mesh, const CGAL::SM_Vertex_index& vertex)
{
    if (nullptr == mesh)
        return { 0, 0, 0 };
    auto vertex_normal = CGAL::Polygon_mesh_processing::compute_vertex_normal(vertex, *mesh);
    return QVector3D(
        static_cast<float>(CGAL::to_double(vertex_normal.x())),
        static_cast<float>(CGAL::to_double(vertex_normal.y())),
        static_cast<float>(CGAL::to_double(vertex_normal.z()))
    );
}

bool CGAL_API::checkConstructedMesh(const std::unique_ptr<Surface_mesh>& mesh, const std::string& filepath)
{
    if (mesh->is_empty()) {
        qCritical() << "Critical: CGAL API mesh constructed from " << filepath.c_str() << " apear to be empty.";
        return false;
    }
    if (!mesh->is_valid()) {
        qCritical() << "Critical: CGAL API mesh constructed from " << filepath.c_str() << " apear to be invalid.";
        return false;
    }
    return true;
}
