#ifndef _SCENE_H_
#define _SCENE_H_

// std
#include <fstream>
#include <algorithm>
#include <list>

// Qt
#include <QtOpenGL>

// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include "dt.h"

// Delaunay triangulation
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Triangulation_vertex_base_2<Kernel> Vertex_base;
typedef My_vertex_base<Vertex_base> Vb;
typedef CGAL::Triangulation_face_base_2<Kernel> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
typedef DT<Kernel, TDS> Triangulation;

#undef min
#undef max

class GlViewer;

class Scene
{
public:
  typedef Kernel::FT FT;
  typedef Kernel::Point_2 Point;


private:
  // Delaunay triangulation
  Triangulation m_dt;
  bool m_view_delaunay_edges;
  bool m_view_voronoi_edges;

  Point m_mouse_pos;

public:    
  Scene()
  {
    m_view_delaunay_edges = true;
    m_view_voronoi_edges = true;
  }

  ~Scene()
  {
  }    

  void clear()
  {
    m_dt.clear();
  }

  void generate_random(unsigned int nb_generators)
  {
    m_dt.generators_random(nb_generators);
  }

  void add_point(const Point& point)
  {
    m_dt.insert(point);
  }

  void set_mouse_pos(const Point& pos) {  m_mouse_pos = pos; }


  void render(GlViewer* viewer)
  {
    // render generators
    m_dt.gl_draw_generators(2.0f, 0, 0, 0); // black dots

    // render Delaunay edges
    if(m_view_delaunay_edges)
      m_dt.gl_draw_delaunay_edges(1.0f, 0, 0, 128); // dark blue line segments

    // render Voronoi edges
    if(m_view_voronoi_edges)
      m_dt.gl_draw_voronoi_edges(1.0f, 0, 128, 0, viewer); // dark green line segments
  }

  void toggle_view_Delaunay_edges() { m_view_delaunay_edges = !m_view_delaunay_edges; }
  void toggle_view_Voronoi_edges() { m_view_voronoi_edges = !m_view_voronoi_edges; }

  void lloyd()
  {
    // to complete
    // m_dt.lloyd(); 
  }

};

#endif // _SCENE_H_
