#include <QtGui>
#include "glviewer.h"
#include "scene.h"
#include <CGAL/assertions_behaviour.h>

GlViewer::GlViewer(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
{
  CGAL::set_error_behaviour(CGAL::ABORT);
  m_scene = NULL;
  m_center_x = m_center_y = 0.5;
  m_scale = 1.0;

  setAutoFillBackground(false);
  setMouseTracking(true);
}

void GlViewer::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  double aspect_ratio = double(height) / double(width);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -aspect_ratio, aspect_ratio, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  update_rectangle();
}

void GlViewer::update_rectangle()
{
  convert_to_world_space(QPoint(0, 0), _x_min, _y_max);
  convert_to_world_space(QPoint(width(), height()), _x_max, _y_min);
}

void GlViewer::initializeGL() {
  glClearColor(1., 1., 1., 0.);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SMOOTH);
}

void GlViewer::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT);
  if (!m_scene) return;

  glPushMatrix();
  glScaled(m_scale, m_scale, m_scale);
  glTranslated(-m_center_x, -m_center_y, 0.0);
  m_scene->render(this);
  glPopMatrix();
}

void GlViewer::wheelEvent(QWheelEvent *event) {
  if (!m_scene) return;
  m_scale += 0.05 * (event->delta() / 120);
  if (m_scale <= 0.0) m_scale = 0.0;
  update_rectangle();
  updateGL();
}

void GlViewer::mousePressEvent(QMouseEvent *event) {
  if (!m_scene) return;
  m_mouse_click = event->pos();

  if (event->button() == Qt::LeftButton) 
  {
    setCursor(QCursor(Qt::PointingHandCursor));
    sample_mouse_path(m_mouse_click);
  } 
  else 
  {
    setCursor(QCursor(Qt::ClosedHandCursor));
  }
}

void GlViewer::mouseMoveEvent(QMouseEvent *event)
{
  if(!m_scene) return;    
  m_mouse_move = event->pos();
  double x, y;
  convert_to_world_space(m_mouse_move, x, y);
  m_scene->set_mouse_pos(Scene::Point(x, y));

  if (event->buttons() == Qt::LeftButton)
  {
    if (m_mouse_move != m_mouse_click)
      sample_mouse_path(m_mouse_move);
  }
  else if(event->buttons() == Qt::RightButton)
  {        
    move_camera(m_mouse_click, m_mouse_move);
  }

  m_mouse_click = m_mouse_move;
  updateGL();
}

void GlViewer::mouseReleaseEvent(QMouseEvent *event) {
  if (!m_scene) return;
  m_mouse_move = event->pos();

  if (event->button() == Qt::LeftButton) 
  {
    if (m_mouse_move != m_mouse_click)
      sample_mouse_path(m_mouse_move);
  } 
  else 
  {
    move_camera(m_mouse_click, m_mouse_move);
  }    

  m_mouse_click = m_mouse_move;
  setCursor(QCursor(Qt::ArrowCursor));
  updateGL();
}

void GlViewer::sample_mouse_path(const QPoint& point)
{
  double x, y;
  convert_to_world_space(point, x, y);

  m_scene->add_point(Scene::Point(x, y));
}

void GlViewer::move_camera(const QPoint& p0, const QPoint& p1)
{
  m_center_x -= double(p1.x() - p0.x()) / double(width());
  m_center_y += double(p1.y() - p0.y()) / double(height()); 
}

void GlViewer::convert_to_world_space(const QPoint& point, double &x, double &y)
{
  double aspect_ratio = double(height()) / double(width());

  x = double(point.x()) / double(width());
  x = (2.0*x - 1.0) / m_scale;
  x += m_center_x;

  y = 1.0 - double(point.y()) / double(height());
  y = (2.0*y - 1.0) * aspect_ratio / m_scale;
  y += m_center_y;
}
