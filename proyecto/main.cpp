#include <QApplication>
#include "dtablausuarios.h"


 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
  	  DTablaUsuarios * dialogo = new DTablaUsuarios();
  	  dialogo->show();

     return app.exec();
 }

