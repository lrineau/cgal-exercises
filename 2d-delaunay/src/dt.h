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
			  const unsigned char blue,
			  Point m_mouse_pos)
  {
    ::glPointSize(point_size);
    Vertex_handle v = Dt2::nearest_vertex(m_mouse_pos);

    ::glBegin(GL_POINTS);
    typename Dt2::Finite_vertices_iterator vit;
    for(vit = Dt2::finite_vertices_begin();
	vit != Dt2::finite_vertices_end();
	vit++)
    {
      const Point& p = vit->point();
      if((Vertex_handle)vit == v) {
	::glColor3ub(255, 0, 0);
      }
      else {
	::glColor3ub(red, green, blue);
      }
      ::glVertex2f(p.x(), p.y());
    }
    ::glEnd();
  }

  // draw delaunay edges
  void gl_draw_delaunay_edges(const float line_width,
			      const unsigned char red,
			      const unsigned char green,
			      const unsigned char blue,
			      bool convex_hull_in_red)
  {
    ::glLineWidth(line_width);

    ::glBegin(GL_LINES);
    Edge_iterator hEdge;
    for(hEdge  = Dt2::edges_begin(); 
	hEdge != Dt2::edges_end(); 
	hEdge++) 
    {
      if(convex_hull_in_red &&
	 Dt2::dimension() == 2 &&
	 ( Dt2::is_infinite(hEdge->first) ||
	   Dt2::is_infinite(hEdge->first->neighbor(hEdge->second)) )
	 )
      {
	::glColor3ub(255, 0, 0);
      } else {
	::glColor3ub(red, green, blue);
      }
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

    typedef typename Kernel::Iso_rectangle_2 Rectangle;
    const Rectangle rect(viewer->x_min(),
			 viewer->y_min(),
			 viewer->x_max(),
			 viewer->y_max());
    ::glBegin(GL_LINES);
    for (typename Dt2::Finite_edges_iterator
	   eit = this->finite_edges_begin(), end = this->finite_edges_end();
	 eit != end; ++eit)
    {
      CGAL::Object o = this->dual(eit);
      Line    l;
      Ray     r;
      Segment s;
      bool s_is_initialized = false;

      using CGAL::cpp11::result_of;
      typedef typename Kernel::Intersect_2 Intersect_2;

      if (CGAL::assign(l,o)) {
	typename result_of<Intersect_2(Line, Rectangle)>::type inter =
	  CGAL::intersection(l, rect);
	if(inter) {
	  if(const Segment* s_ptr = boost::get<Segment>(&*inter)) {
	    s = *s_ptr;
	    s_is_initialized = true;
	  }
	}
      } else if (CGAL::assign(r,o)) {
	typename result_of<Intersect_2(Ray, Rectangle)>::type inter =
	  CGAL::intersection(r, rect);
	if(inter) {
	  if(const Segment* s_ptr = boost::get<Segment>(&*inter)) {
	    s = *s_ptr;
	    s_is_initialized = true;
	  }
	}
      }
      if (s_is_initialized || CGAL::assign(s,o)) {
	::glVertex2d(s.source().x(), s.source().y());
	::glVertex2d(s.target().x(), s.target().y());
      }
    }
    ::glEnd();
  }
};

#endif // TRIANGULATION_H
