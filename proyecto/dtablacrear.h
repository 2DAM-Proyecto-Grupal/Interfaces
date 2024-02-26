
#ifndef DTABLACREAR_H
#define DTABLACREAR_H
#include "ui_dtablacrear.h"

#include <QVector>
#include <QDialog>
#include <QHBoxLayout>
#include "usuarios.h"

class DTablaCrear : public QDialog, public Ui::DTablaCrear {
Q_OBJECT

public:
	DTablaCrear(QWidget *parent = NULL);
	
	Usuarios * usuarioNuevo;

public slots:
	void slotGuardar();


signals:

	void senyalCrear(Usuarios *);
};

#endif 
