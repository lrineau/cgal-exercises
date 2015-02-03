#ifndef _WINDOW_
#define _WINDOW_

// Qt
#include <QMainWindow>
#include "ui_delaunay.h"

class Scene;

class MainWindow : public QMainWindow, public Ui_MainWindow
{
  Q_OBJECT

private:
  Scene* m_scene;

  unsigned int maxNumRecentFiles;
  QAction* recentFilesSeparator;
  QVector<QAction*> recentFileActs;

public:
  MainWindow();
  ~MainWindow();

  void update();

protected:

protected slots:

  // drag & drop
  void closeEvent(QCloseEvent *event);

public slots:

  // Data
  void on_actionClear_triggered();
  void on_actionRandom_triggered();

  // View
  void on_actionView_Delaunay_edges_toggled();
  void on_actionView_Voronoi_edges_toggled();
  void on_actionConvexHull_toggled();
  void on_actionDrawCentroids_toggled();

  // algorithms
  void on_actionLloyd_triggered();
};

#endif // _WINDOW_
