
#include "dtablacrear.h"
#include <QDebug>

DTablaCrear::DTablaCrear(QWidget *parent): QDialog(parent){
		setupUi(this);
		
		
		usuarioNuevo = new Usuarios();
		
		connect(bGuardar, SIGNAL(clicked()),
					this, SLOT(slotGuardar()));
					
		connect(bCancelar,SIGNAL(clicked()),
					this, SLOT(close()));
	
}


void DTablaCrear::slotGuardar(){
	


	usuarioNuevo->pass = lContra->text();
	usuarioNuevo->name = lNombre->text();
	usuarioNuevo->login = lLogin->text();
	usuarioNuevo->karma = sKarma->value();

	emit senyalCrear(usuarioNuevo);
	
	close();
}
