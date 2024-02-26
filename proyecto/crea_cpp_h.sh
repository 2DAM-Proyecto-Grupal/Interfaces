#!/bin/bash

echo "Creador de ficheros .h y .cpp para un diálog creado con designer"
echo "Aviso: una clase \"Dialogo\" debe estar en un fichero \"dialogo.ui\" "

read -p "Introduce el nombre de la clase: " clase
read -p "Introduce el nombre de la clase padre (QDialog):  " padre

if [ "$padre" == "" ] ; then 
	padre="QDialog"
fi

claseMinusculas=$( echo $clase | tr [A-Z] [a-z] )
claseMayusculas=$( echo $clase | tr [a-z] [A-Z] )

puntoH="${claseMinusculas}.h"
puntoCPP="${claseMinusculas}.cpp"


if ! [ -f ${claseMinusculas}.ui ] ; then
	echo fichero ${claseMinusculas}.ui no encontrado. Abortando
	exit -1
fi

if [ -f ${puntoH} ] || [ -f ${puntoCPP} ] ; then
	read -n1 -p "Los ficheros existen los borro (s/n)" borrar ;
	if [[ "$borrar" =~  [sSyY] ]] ; then
		mv ${puntoH} ${puntoH}.bak  2>/dev/null
		mv ${puntoCPP} ${puntoCPP}.bak 2>/dev/null
	else
		echo " Abortando "
		exit 0
	fi
fi



cat >${puntoH} <<EOF

#ifndef ${claseMayusculas}_H
#define ${claseMayusculas}_H
#include "ui_${claseMinusculas}.h"

#include <QVector>
#include <${padre}>
#include <QHBoxLayout>

class ${clase} : public ${padre}, public Ui::${clase} {
Q_OBJECT

public:
	${clase}(QWidget *parent = NULL);

public slots:
	void slotEjemplo();

};

#endif 
EOF



cat >${puntoCPP} <<EOF

#include "${puntoH}"
#include <QDebug>

${clase}::${clase}(QWidget *parent): ${padre}(parent){
		setupUi(this);

	
}


void ${clase}::slotEjemplo(){

}

EOF

read -p "Este es un diálogo principal (crear main.cpp)" -n1 principal
if  ! [[ "$principal" =~  [sSyY] ]] ; then
	echo Hecho
	exit 0
fi

cat >main.cpp <<EOF
#include <QApplication>
#include "${puntoH}"


 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
  	  ${clase} * dialogo = new ${clase}();
  	  dialogo->show();

     return app.exec();
 }

EOF


echo "Hecho"
