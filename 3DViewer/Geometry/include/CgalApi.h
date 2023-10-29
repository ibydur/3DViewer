#pragma once

#include <QVector3D.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/OBJ.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;
typedef CGAL::Surface_mesh<Point_3> Surface_mesh;

namespace CGAL_API {
   std::unique_ptr<Surface_mesh> constructMeshFromObj(const std::string& file_path);
   QVector3D computeVertexNormal(const std::unique_ptr<Surface_mesh>& mesh, const CGAL::SM_Vertex_index& vertex);
   bool checkConstructedMesh(const std::unique_ptr<Surface_mesh>& mesh, const std::string& filepath);
}
