

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/boost/graph/properties_Polyhedron_3.h>
#include <CGAL/boost/graph/iterator.h>
#include <CGAL/IO/Polyhedron_iostream.h>

#include <iostream>
#include <boost/foreach.hpp>

typedef CGAL::Simple_cartesian<double>     Kernel;
typedef Kernel::Point_3                    Point_3;
typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;

typedef boost::graph_traits<Polyhedron>::vertex_iterator vertex_iterator;
typedef boost::graph_traits<Polyhedron>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Polyhedron>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<Polyhedron>::edge_descriptor edge_descriptor;
typedef boost::graph_traits<Polyhedron>::face_descriptor face_descriptor;

int main() {

  Polyhedron P;
  std::cin >> P;
 
  // take any face of the cube
  
  face_descriptor f; // = ...
  halfedge_descriptor hd = halfedge(f,P);


  vertex_iterator b,e;
  for ( boost::tie(b,e) = vertices(P); b!= e; ++b)
    std::cout << (*b)->point() << std::endl;
  return 0;
}
