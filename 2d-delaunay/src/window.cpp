// Qt
#include <QtGui>
#include <QInputDialog>

#include "window.h"
#include "scene.h"

MainWindow::MainWindow() : 
  QMainWindow(), Ui_MainWindow(), 
  maxNumRecentFiles(15), recentFileActs(15)
{
  setupUi(this);
    
  // init scene
  m_scene = new Scene;
  viewer->set_scene(m_scene);
    
  // Handling actions
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
  delete m_scene;
}

void MainWindow::update()
{
  viewer->repaint();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
  event->accept();
}



void MainWindow::on_actionClear_triggered()
{
  m_scene->clear();
  update();
}

void MainWindow::on_actionRandom_triggered()
{
  int nb = QInputDialog::getInteger(this,
				    tr("Generate random points"),
				    tr("Number of points"),
				    100);
  if(nb > 0) {
    m_scene->generate_random((unsigned int)nb);
    update();
  }
}

void MainWindow::on_actionView_Delaunay_edges_toggled()
{
  m_scene->toggle_view_Delaunay_edges();
  update();
}

void MainWindow::on_actionView_Voronoi_edges_toggled()
{
  m_scene->toggle_view_Voronoi_edges();
  update();
}

void MainWindow::on_actionLloyd_triggered()
{
  m_scene->lloyd();
  update();
}

