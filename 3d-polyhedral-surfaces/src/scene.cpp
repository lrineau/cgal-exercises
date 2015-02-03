#include "scene.h"

#include <iostream>
#include <fstream>

#include <QFileInfo>
#include <QTextStream>
#include <QInputDialog>
#include <QtCore/qglobal.h>
#include <QApplication>

#include <QGLViewer/qglviewer.h>
#include "copier.h"


Scene::Scene()
{
	// view options
	m_view_facets = true;
	m_view_edges = true;
	m_smooth = false;
}

Scene::~Scene()
{
}

void Scene::render()
{
	::glDisable(GL_LIGHTING);

	if(m_view_edges)
		m_polyhedron.gl_draw_edges(1.0f, 128, 128, 128);
}

int Scene::open(QString filename)
{
	QApplication::setOverrideCursor(QCursor(::Qt::WaitCursor));

	QFileInfo fileinfo(filename);
	std::ifstream in(filename.toUtf8());

	if(!in || !fileinfo.isFile() || ! fileinfo.isReadable())
	{
		std::cerr << "unable to open file" << std::endl;
		QApplication::restoreOverrideCursor();
		return -1;
	}

	// read polyhedron
	std::cout << "read polyhedron...";
	in >> m_polyhedron;
	std::cout << "done" << std::endl;

	// compute normals
	m_polyhedron.compute_normals();

	if(!in)
	{
		std::cerr << "invalid OFF file" << std::endl;
		QApplication::restoreOverrideCursor();
		return -1;
	}

	QApplication::restoreOverrideCursor();
	return 0;
}

void Scene::copy()
{
  typedef CCopier<Polyhedron, Kernel> Copier;
  Polyhedron m_copy;
  Copier copier;
  copier.copy(m_polyhedron, m_copy);
}

void Scene::dualize()
{
	// TO COMPLETE
}


