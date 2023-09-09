#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/OBJ.h>

#include <QVector3D.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Surface_mesh<Point_3> Surface_mesh;

namespace CGAL_API {
   std::unique_ptr<Surface_mesh> constructMeshFromObj(const std::string& file_path);
   QVector<QVector3D> loadMeshToVector(const std::unique_ptr<Surface_mesh>& mesh);
}

