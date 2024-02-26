
#ifndef DTABLAMODIFICAR_H
#define DTABLAMODIFICAR_H
#include "ui_dtablamodificar.h"

#include <QVector>
#include <QDialog>
#include <QHBoxLayout>
#include "usuarios.h"

class DTablaModificar : public QDialog, public Ui::DTablaModificar {
Q_OBJECT

public:
	DTablaModificar(Usuarios * usuario, QWidget *parent = NULL);

	Usuarios * usuarioBasico;

	Usuarios * usuarioModificado;

public slots:
	void slotGuardar();


signals:

	void senyalGuardar(Usuarios *);
};

#endif 
