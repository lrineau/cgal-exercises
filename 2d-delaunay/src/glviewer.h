#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QPaintEvent>

class Scene;

class GlViewer : public QGLWidget 
{
  Q_OBJECT
    
private:
  Scene* m_scene;
    
  // camera
  double m_scale;
  double m_center_x, m_center_y;
  double _x_min, _x_max, _y_min, _y_max;
    
  // mouse
  QPoint m_mouse_click, m_mouse_move;
    
public:
  GlViewer(QWidget *parent);
    
  void set_scene(Scene* pScene) { m_scene = pScene; }
    
  void set_camera(const double x, const double y, const double s) 
  {
    m_center_x = x;
    m_center_y = y;
    m_scale = s;
  }

  double x_min() const { return _x_min; };
  double x_max() const { return _x_max; };
  double y_min() const { return _y_min; };
  double y_max() const { return _y_max; };

protected:
  // GL
  void paintGL();
  void initializeGL();
  void resizeGL(int width, int height);
    
  // mouse
  void wheelEvent(QWheelEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
    
  void sample_mouse_path(const QPoint& point);
  void move_camera(const QPoint& p0, const QPoint& p1);
  void update_rectangle();
  void convert_to_world_space(const QPoint& point, double &x, double &y);
};

#endif
