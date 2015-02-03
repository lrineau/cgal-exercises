#ifndef	_POLYGON_MESH_
#define	_POLYGON_MESH_

// CGAL	stuff
#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <list>

// a refined facet with a normal 
template <class	Refs,	class	T, class P,	class	Norm>
class	Enriched_facet : public	CGAL::HalfedgeDS_face_base<Refs, T>
{
	// normal
	Norm m_normal;

public:

	// life	cycle
	Enriched_facet()
	{
	}

	// normal
	typedef	Norm Normal_3;
	Normal_3&	normal() { return	m_normal;	}
	const	Normal_3&	normal() const { return	m_normal;	}
};

// a refined halfedge with a general tag 
template <class	Refs,	class	Tprev, class Tvertex,	class	Tface, class Norm>
class	Enriched_halfedge	:	public CGAL::HalfedgeDS_halfedge_base<Refs,Tprev,Tvertex,Tface>
{
private:

	// general-purpose tag
	int m_tag;

public:

	// life	cycle
	Enriched_halfedge()
	{
	}

	// tag
	int& tag() { return m_tag; }
	const int& tag() const { return m_tag; }
	void tag(const int& t) { m_tag = t; }
};



// a refined vertex with a normal and a tag
template <class	Refs,	class	T, class P,	class	Norm>
class	Enriched_vertex	:	public CGAL::HalfedgeDS_vertex_base<Refs,	T, P>
{
	// normal
	Norm m_normal;

	// general-purpose tag
	int m_tag;

public:
	// life	cycle
	Enriched_vertex()	 {}
	// repeat	mandatory	constructors
	Enriched_vertex(const	P& pt)
		:	CGAL::HalfedgeDS_vertex_base<Refs, T,	P>(pt)
	{
	}

	// tag
	int& tag() { return m_tag; }
	const int& tag() const { return m_tag; }
	void tag(const int& t) { m_tag = t; }

	// normal
	typedef	Norm Normal_3;
	Normal_3&	normal() { return	m_normal;	}
	const	Normal_3&	normal() const { return	m_normal;	}
};

// A redefined items class for the Polyhedron_3	
// with	a	refined	vertex class that	contains a 
// member	for	the	normal vector	and	a	refined
// facet with	a	normal vector	instead	of the 
// plane equation	(this	is an	alternative	
// solution	instead	of using 
// Polyhedron_traits_with_normals_3).

struct Enriched_items	:	public CGAL::Polyhedron_items_3
{
	// wrap	vertex
	template <class	Refs,	class	Traits>
	struct Vertex_wrapper
	{
		typedef	typename Traits::Point_3	Point;
		typedef	typename Traits::Vector_3	Normal;
		typedef	Enriched_vertex<Refs,
			CGAL::Tag_true,
			Point,
			Normal>	Vertex;
	};

	// wrap	face
	template <class	Refs,	class	Traits>
	struct Face_wrapper
	{
		typedef	typename Traits::Point_3	Point;
		typedef	typename Traits::Vector_3	Normal;
		typedef	Enriched_facet<Refs,
			CGAL::Tag_true,
			Point,
			Normal> Face;
	};

	// wrap	halfedge
	template <class	Refs,	class	Traits>
	struct Halfedge_wrapper
	{
		typedef	typename Traits::Vector_3	Normal;
		typedef	Enriched_halfedge<Refs,
			CGAL::Tag_true,
			CGAL::Tag_true,
			CGAL::Tag_true,
			Normal>	Halfedge;
	};
};

struct Vertex_normal;
struct Facet_normal;

// Enriched polyhedron
template <class	kernel,	class items>
class Enriched_polyhedron	: public CGAL::Polyhedron_3<kernel, items>
{
public :
	typedef	typename CGAL::Polyhedron_3<kernel, items> Base;

	typedef	typename Base::Halfedge_iterator Halfedge_iterator;
	typedef typename Base::Halfedge_handle Halfedge_handle;

	typedef	typename kernel::FT	FT;
	typedef	typename kernel::Point_3 Point;

public :

	// LIFE CYCLE ============================================

	Enriched_polyhedron()	
	{
	}
	virtual	~Enriched_polyhedron() 
	{
	}

	// RENDERING =============================================

	// draw	edges
	void gl_draw_edges(const float line_width,
		const unsigned char r, 
		const unsigned char g, 
		const unsigned char b)
	{
		::glBegin(GL_LINES);

		::glLineWidth(line_width);
		::glColor3ub(r, g, b); 

		typename Base::Halfedge_iterator it;
		for(it = Base::edges_begin();
			it != Base::edges_end();
			it++)
		{
			Halfedge_handle he = it;
			const Point& p1 = he->opposite()->vertex()->point();
			const Point& p2 = he->vertex()->point();
			::glVertex3d(p1[0], p1[1], p1[2]);
			::glVertex3d(p2[0], p2[1], p2[2]);
		}

		::glEnd();
	}

	// NORMALS ===============================================

	// normals (per	facet, then	per	vertex)
	void compute_normals()
	{
		compute_normals_per_facet();
		compute_normals_per_vertex();
	}
	void compute_normals_per_facet()
	{
		std::for_each(Base::facets_begin(), Base::facets_end(), Facet_normal());
	}
	void compute_normals_per_vertex()
	{
		std::for_each(Base::vertices_begin(), Base::vertices_end(), Vertex_normal());
	}
};

// compute facet normal	
struct Facet_normal	// (functor)
{
	template <class	Facet>
	void operator()(Facet& f)
	{
		typename Facet::Normal_3 sum = CGAL::NULL_VECTOR;
		typename Facet::Halfedge_around_facet_circulator h = f.facet_begin();
		do
		{
			typename Facet::Normal_3 normal	= CGAL::cross_product(
				h->next()->vertex()->point() - h->vertex()->point(),
				h->next()->next()->vertex()->point() - h->next()->vertex()->point());
			double sqnorm	=	normal * normal;
			if(sqnorm	!= 0)
				normal = normal	/	(double)std::sqrt(sqnorm);
			sum	=	sum	+	normal;
		}
		while(++h != f.facet_begin());
		double sqnorm = sum * sum;
		if(sqnorm	!= 0.0)
			f.normal() = sum / std::sqrt(sqnorm);
		else
		{
			f.normal() = CGAL::NULL_VECTOR;
			std::cerr << "degenerated facet" << std::endl;
		}
	}
};


// compute vertex	normal 
struct Vertex_normal //	(functor)
{
	template <class	Vertex>
	void operator()(Vertex&	v)
	{
		typename Vertex::Normal_3	normal = CGAL::NULL_VECTOR;
		typename Vertex::Halfedge_around_vertex_const_circulator	he	=	v.vertex_begin();
		typename Vertex::Halfedge_around_vertex_const_circulator	end	=	he;
		CGAL_For_all(he, end)	
			if(!he->is_border())
				normal = normal	+	he->facet()->normal();
		double	sqnorm = normal * normal;
		if(sqnorm != 0.0)
			v.normal() = normal	/	(double)std::sqrt(sqnorm);
		else
			v.normal() = CGAL::NULL_VECTOR;
	}
};

#endif //	_POLYGON_MESH_
