
#ifndef DTABLAUSUARIOS_H
#define DTABLAUSUARIOS_H
#include "ui_dtablausuarios.h"

#include <QVector>
#include <QDialog>
#include <QHBoxLayout>
#include <QAbstractTableModel>
#include <QVariant>
#include <QModelIndex>
#include "dtablamodificar.h"

#include <QTimer>
#include <QTableWidgetItem>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QMessageBox>
#include "dtablacrear.h"

class ModeloTabla;

class DTablaUsuarios : public QDialog, public Ui::DTablaUsuarios {
Q_OBJECT

public:
	DTablaUsuarios(QWidget *parent = NULL);
	
	ModeloTabla * modelo;
	

public slots:
 void slotEliminar();


};

class ModeloTabla : public QAbstractTableModel {
Q_OBJECT
public:

	QNetworkAccessManager * manager;
	
	QByteArray requestData(QString dbName, QString pass);
	QByteArray putData(QString databaseName, QString password, Usuarios * usuario);
	QByteArray deleteData(QString databaseName, QString password, int uid);
	QByteArray postData(QString databaseName, QString password, Usuarios * usuario);
	
	bool procesarRespuesta(const QByteArray &respuesta);
	
	QVector<Usuarios*> listaUsuarios;
	
	void actualizar();
	DTablaModificar * dialogo;
	QTimer * temporizador;
	
	void eliminarUsuario(int);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;


public slots:
	void slotLanzarDialogo(const QModelIndex &);
	void slotModificar(Usuarios*);
	void slotLeerUsuarios();

	void slotRespuestaFinalizada(QNetworkReply *);
	
	void slotDialogoCrear();
	
	void slotCrear(Usuarios* usu);

signals:

	void mandarSenyal(Usuarios*);
};


#endif 
