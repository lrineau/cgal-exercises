#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <CGAL/Qt/DemosMainWindow.h>

class QDragEnterEvent;
class QDropEvent;

class Viewer;
class Scene;

namespace Ui {
	class MainWindow;
}

class MainWindow : public CGAL::Qt::DemosMainWindow
{
	Q_OBJECT
private:
	Ui::MainWindow* ui;

	Viewer* m_pViewer;
	Scene* m_pScene;

public:
	MainWindow(QWidget* parent = 0);
	~MainWindow();

	public slots:
		void updateViewerBBox();

		// drag & drop
		void dropEvent(QDropEvent *event);
		void closeEvent(QCloseEvent *event);
		void dragEnterEvent(QDragEnterEvent *event);

		protected slots:    

			// settings
			void quit();
			void readSettings();
			void writeSettings();

			// file menu
			void on_actionOpen_triggered();
			void open(QString filename);

			// view menu
			void on_actionView_edges_toggled();
			void on_actionView_smooth_toggled();

			// algorithms menu
			void on_actionCopy_triggered();
			void on_actionDualize_triggered();
};

#endif // ifndef MAINWINDOW_H
