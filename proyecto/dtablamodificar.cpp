
#include "dtablamodificar.h"
#include <QDebug>

DTablaModificar::DTablaModificar(Usuarios *usuario,QWidget *parent): QDialog(parent){
		setupUi(this);
		
		usuarioBasico = usuario;
		

		usuarioModificado = new Usuarios();

		lNombre->setText(usuario->name);
		lLogin->setText(usuario->login);
		sKarma->setValue(usuario->karma);
		
		connect(bGuardar, SIGNAL(clicked()),
					this, SLOT(slotGuardar()));
					
		connect(bCancelar,SIGNAL(clicked()),
					this, SLOT(close()));
	
}


void DTablaModificar::slotGuardar(){
	


	usuarioModificado->uid = usuarioBasico->uid;
	usuarioModificado->name = lNombre->text();
	usuarioModificado->login = lLogin->text();
	usuarioModificado->is_premium = usuarioBasico->is_premium;
	usuarioModificado->karma = sKarma->value();

	emit senyalGuardar(usuarioModificado);
	
	close();
}

