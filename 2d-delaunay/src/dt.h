#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <CGAL/basic.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include "glviewer.h"

// a base vertex with attributes
template <class Vbb>
class My_vertex_base : public Vbb
{
public:
  typedef Vbb Vertex_base;
  typedef typename Vertex_base::Point Point;

private:
  // additional member data
  bool m_input;

public:
  bool& input() { return m_input; }

public:
  My_vertex_base()
    : Vertex_base()
  {
  }
  My_vertex_base(const Point & p, void* f)
    : Vertex_base(p,f)
  {
  }
  My_vertex_base(const Point & p)
    : Vertex_base(p)
  {
  }
};


template <class Kernel, class TDS>
class DT : public CGAL::Delaunay_triangulation_2<Kernel,TDS>
{
public:

  // typedefs for basic primitives
  typedef typename Kernel::FT FT;
  typedef typename Kernel::Ray_2      Ray;
  typedef typename Kernel::Line_2     Line;
  typedef typename Kernel::Point_2    Point;
  typedef typename Kernel::Vector_2   Vector;
  typedef typename Kernel::Segment_2  Segment;
  typedef typename Kernel::Triangle_2 Triangle;

  typedef typename CGAL::Delaunay_triangulation_2<Kernel,TDS> Dt2;

  // handles
  typedef typename Dt2::Face_handle         Face_handle;
  typedef typename Dt2::Vertex_handle       Vertex_handle;

  // iterators
  typedef typename Dt2::Face_iterator       Face_iterator;
  typedef typename Dt2::Edge_iterator       Edge_iterator;
  typedef typename Dt2::Vertex_iterator     Vertex_iterator;

  // circulators
  typedef typename Dt2::Edge_circulator     Edge_circulator;

private:

public:
  DT() 
  {
  }

  virtual ~DT() 
  {
    Dt2::clear();
  }

public:

  // random (uniform)
  void generators_random(unsigned int nb_generators)
  {
    Dt2::clear();
    for(unsigned int i = 0; i < nb_generators; i++)
      this->insert(Point(r(), r()));
  }

  // random number between zero and max
  FT r(FT max = 1.0) { return max * (FT)rand() / (FT)RAND_MAX; }

  // OPENGL DRAWINGS FUNCTIONS

  // draw generators
  void gl_draw_generators(const float point_size,
			  const unsigned char red,
			  const unsigned char green,
			  const unsigned char blue)
  {
    ::glColor3ub(red, green, blue);
    ::glPointSize(point_size);

    ::glBegin(GL_POINTS);
    typename Dt2::Point_iterator it;
    for(it = Dt2::points_begin(); 
	it != Dt2::points_end(); 
	it++)
    {
      const Point& p = *it;
      ::glVertex2f(p.x(), p.y());
    }
    ::glEnd();
  }

  // draw delaunay edges
  void gl_draw_delaunay_edges(const float line_width,
			      const unsigned char red,
			      const unsigned char green,
			      const unsigned char blue)
  {
    ::glColor3ub(red, green, blue);
    ::glLineWidth(line_width);

    ::glBegin(GL_LINES);
    Edge_iterator hEdge;
    for(hEdge  = Dt2::edges_begin(); 
	hEdge != Dt2::edges_end(); 
	hEdge++) 
    {
      const Point& p1 = (*hEdge).first->vertex(Dt2::ccw((*hEdge).second))->point();
      const Point& p2 = (*hEdge).first->vertex(Dt2::cw((*hEdge).second))->point();
      ::glVertex2d(p1.x(), p1.y());
      ::glVertex2d(p2.x(), p2.y());
    }
    ::glEnd();
  }

  void gl_draw_voronoi_edges(const float line_width,
			     const unsigned char red,
			     const unsigned char green,
			     const unsigned char blue,
			     GlViewer* viewer)
  {
    ::glColor3ub(red, green, blue);
    ::glLineWidth(line_width);

    // TO COMPLETE
  }
};

#endif // TRIANGULATION_H
