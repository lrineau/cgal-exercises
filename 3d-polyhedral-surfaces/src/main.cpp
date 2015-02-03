#include "window.h"
#include <QApplication>

int main(int argc, char **argv)
{
	srand(0);
  QApplication app(argc, argv);
  app.setOrganizationDomain("inria.fr");
  app.setOrganizationName("Inria");
  app.setApplicationName("Polyhedral surfaces");

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}
