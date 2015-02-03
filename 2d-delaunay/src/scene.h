#ifndef _SCENE_H_
#define _SCENE_H_

// std
#include <vector>

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
  bool m_draw_convex_hull_in_red;
  bool m_draw_centroids;

  Point m_mouse_pos;
  std::vector<Point> m_centroids;

public:    
  Scene()
  {
    m_view_delaunay_edges = true;
    m_view_voronoi_edges = true;
    m_draw_convex_hull_in_red = false;
  }

  ~Scene()
  {
  }    

  void clear()
  {
    m_dt.clear();
    update_centroids();
  }

  void generate_random(unsigned int nb_generators)
  {
    m_dt.generators_random(nb_generators);
    update_centroids();
  }

  void update_centroids()
  {
    if(m_dt.dimension() == 2) {
      m_centroids.clear();
      m_centroids.reserve(m_dt.number_of_vertices());
      for(Triangulation::Finite_vertices_iterator
	    vit = m_dt.finite_vertices_begin(),
	    end = m_dt.finite_vertices_end();
	  vit != end; ++vit)
      {
	m_centroids.push_back(centroid_voronoi_cell(vit));
	std::cerr << m_centroids.back() << "\n";
      }
      assert(m_centroids.size() == m_dt.number_of_vertices());
    }
  }

  void add_point(const Point& point)
  {
    m_dt.insert(point);
    update_centroids();
  }

  void set_mouse_pos(const Point& pos) {  m_mouse_pos = pos; }

  void draw_centroids() {
    ::glColor3ub(0, 0, 255);
    ::glBegin(GL_POINTS);
    for(std::size_t i = 0, end = m_centroids.size(); i < end; ++i)
    {
      ::glVertex2f(m_centroids[i].x(),
		   m_centroids[i].y());
    }
    ::glEnd();
  }

  void draw_hovered_face()
  {
    Triangulation::Face_handle fh = m_dt.locate(m_mouse_pos);
    if(fh != NULL && !m_dt.is_infinite(fh)) {
      const Point& pa = fh->vertex(0)->point();
      const Point& pb = fh->vertex(1)->point();
      const Point& pc = fh->vertex(2)->point();
      ::glColor3ub(255, 255, 153);
      ::glBegin(GL_TRIANGLES);
      ::glVertex2f(pa.x(), pa.y());
      ::glVertex2f(pb.x(), pb.y());
      ::glVertex2f(pc.x(), pc.y());
      ::glEnd();
    }
  }

  void render(GlViewer* viewer)
  {
    if(m_dt.dimension() == 2) {
      draw_hovered_face();
    }

    if(m_draw_centroids) {
      draw_centroids();
    }

    // render generators
    m_dt.gl_draw_generators(2.0f, 0, 0, 0, m_mouse_pos); // black dots

    // render Delaunay edges
    if(m_view_delaunay_edges)
      m_dt.gl_draw_delaunay_edges(1.0f, 0, 0, 128,
				  m_draw_convex_hull_in_red); // dark blue line segments

    // render Voronoi edges
    if(m_view_voronoi_edges)
      m_dt.gl_draw_voronoi_edges(1.0f, 0, 128, 0, viewer); // dark green line segments
  }

  void toggle_view_Delaunay_edges() { m_view_delaunay_edges = !m_view_delaunay_edges; }
  void toggle_view_Voronoi_edges() { m_view_voronoi_edges = !m_view_voronoi_edges; }
  void toggle_convex_hull_in_red() { m_draw_convex_hull_in_red = !m_draw_convex_hull_in_red; }
  void toggle_draw_centroids() { m_draw_centroids = !m_draw_centroids; }

  Point centroid_voronoi_cell(Triangulation::Vertex_handle v)
  {
    Kernel::Vector_2 sum = CGAL::NULL_VECTOR;
    double mass = 0.;

    Triangulation::Face_circulator f_circ = m_dt.incident_faces(v);
    Triangulation::Face_circulator f_end = f_circ;
    do {
      if(m_dt.is_infinite(f_circ)) return v->point();

      Point circumcenter = m_dt.dual(f_circ);
      int index_of_v = f_circ->index(v);
      Point a = f_circ->vertex(m_dt.cw(index_of_v))->point();
      Point b = f_circ->vertex(m_dt.ccw(index_of_v))->point();

      Kernel::Triangle_2 tri(a, b, v->point());
      double area = tri.area();
      mass += area;
      sum = sum + area * (circumcenter - v->point());
    } while(++f_circ != f_end);
    return v->point() + sum / mass;
  }

  void lloyd()
  {
    m_dt.clear();
    m_dt.insert(m_centroids.begin(),
		m_centroids.end());
    update_centroids();
  }

};

#endif // _SCENE_H_
