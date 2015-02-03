#ifndef SCENE_H
#define SCENE_H

#include <QtOpenGL/qgl.h>
#include <QString>
#include <list>



// CGAL
#include <CGAL/basic.h>
#include <CGAL/Simple_cartesian.h>
#include "polyhedron.h"
#include <CGAL/IO/Polyhedron_iostream.h>

#include "types.h"

class Scene
{
private:

	// Polyhedral surface
	Polyhedron m_polyhedron;

	// rendering options
	bool m_smooth;
	bool m_view_edges;
	bool m_view_facets;

public: // life cycle
	Scene();
	virtual ~Scene();

	// file menu
	int open(QString filename);

	// algorithms menu
	void copy();
	void dualize();

	// toggle rendering options
	void toggle_view_smooth()  { m_smooth = !m_smooth; } 
	void toggle_view_facets()  { m_view_facets = !m_view_facets; } 
	void toggle_view_edges()   { m_view_edges = !m_view_edges; } 

	// rendering
	void render(); 
	void render_edges();
};

#endif // SCENE_H
