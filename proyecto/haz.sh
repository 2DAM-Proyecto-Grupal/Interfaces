#!/bin/bash
make clean
rm *.pro
qmake -project
fichero=$( ls *.pro )
echo "QT += widgets network" >> $fichero
qmake
make
echo "pon el && ejecutable para acelerar"
