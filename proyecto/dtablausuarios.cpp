
#include "dtablausuarios.h"
#include <QDebug>

DTablaUsuarios::DTablaUsuarios(QWidget *parent): QDialog(parent){
		setupUi(this);
		
		modelo = new ModeloTabla();
		
		tabla->setModel(modelo);
		
		
		modelo->manager = new QNetworkAccessManager(this);
		
		connect(modelo->manager, SIGNAL(finished(QNetworkReply * )),
     				modelo,SLOT(slotRespuestaFinalizada(QNetworkReply *)));
		
		
		connect(tabla,SIGNAL(activated(const QModelIndex &)),
					modelo, SLOT(slotLanzarDialogo(const QModelIndex &)));
		
		connect(bCrear,SIGNAL(clicked()),
					modelo, SLOT(slotDialogoCrear()));
		
		modelo->slotLeerUsuarios();
		
		modelo->temporizador = new QTimer();
		
		
		connect(modelo->temporizador, SIGNAL(timeout()),
						modelo, SLOT(slotLeerUsuarios()));
						
		//connect(modelo, SIGNAL(mandarSenyal(Usuarios *)),
			//	conexion, SLOT (slotModificar(Usuarios*)));
				
		//connect(conexion, SIGNAL(usuarioModificado(Usuarios*)),
			//	 modelo, SLOT(usuarioModificado(Usuarios*)));
				 
		connect(bEliminar, SIGNAL(clicked()),
					this, SLOT(slotEliminar()));


	
}

void DTablaUsuarios::slotEliminar(){
	
	QModelIndex index = tabla->currentIndex();
	
	int uid = modelo->listaUsuarios.at(index.row())->uid;
	
	if (uid != 2){
		modelo->eliminarUsuario(uid);
	} else {
		int respuesta;
		respuesta = QMessageBox::critical(this,"Usuario Admin","No se puede borrar el usuario admin");
		
	}
}

void ModeloTabla::slotLeerUsuarios(){

	QString databaseName = "Proyecto";
   QString password = "admin";
	
   QByteArray data = requestData(databaseName,password);
   
   QNetworkRequest request;
   request.setUrl(QUrl("http://192.168.1.37:8069/jsonrpc"));
   request.setRawHeader(
		QByteArray("Content-Type"),
		QByteArray("application/json"));
		
	QNetworkReply *reply = manager->post(request, data);

}

QByteArray ModeloTabla::requestData(QString databaseName, QString password){

	 QJsonObject jsonObject;
    jsonObject.insert("jsonrpc", "2.0");
    jsonObject.insert("method", "call");
    
    QJsonObject params;
    params.insert("service", "object");
    params.insert("method", "execute");
    
    QJsonArray args;
    args.append(databaseName); // "jessica2dam" nom de la base de datos
    args.append(2);    // es el usuari
    args.append(password); // "1234" password
    args.append("res.users");
    args.append("search_read");
    
    QJsonArray emptyArray;
    args.append(emptyArray);
    
    QJsonArray fields;
    fields.append("id");
    fields.append("name");
    fields.append("login");
    fields.append("is_premium");
    fields.append("karma");
    args.append(fields);
    
    params.insert("args", args);
	jsonObject.insert("params", params);
	
   quint64 value64;
   value64 = QRandomGenerator().generate64();
   
	jsonObject.insert("id", QString::number(value64));
    
    QByteArray postData = QJsonDocument(jsonObject).toJson();
    qDebug().noquote()<< QString(postData);
    
     return postData;
}

void ModeloTabla::slotRespuestaFinalizada(QNetworkReply* resp){
    QByteArray data = resp->readAll( );
    
    qDebug() << "Error: " << resp->error();

	
	if (! procesarRespuesta(data) )
   	 return;
   	   
}

bool ModeloTabla::procesarRespuesta(const QByteArray &respuesta){
 		 
 	QJsonDocument responseData;
 	responseData = QJsonDocument::fromJson(respuesta);
  	QJsonObject jsonResponse = responseData.object();

	if(! jsonResponse.contains("result")){
	 		qDebug() << "No hay resul";
	 		return false;
	 }
	 		
	QJsonArray result = jsonResponse["result"].toArray();
	
	listaUsuarios.clear();
	for(int i = 0; i < result.size(); i++){
		 QJsonObject partner = result[i].toObject();
		 Usuarios *usuario = new Usuarios();
		 usuario->uid = partner["id"].toInt();
		 usuario->name = partner["name"].toString();
		 usuario->login = partner["login"].toString();
		 usuario->is_premium = partner["is_premium"].toBool();
		 usuario->karma = partner["karma"].toInt();
		 listaUsuarios.append(usuario);    
		 qDebug() << "Karma " << listaUsuarios.at(i)->karma;
	}

	emit layoutChanged();
	
  			 
  	  	return true;
  	  	
}



void ModeloTabla::slotLanzarDialogo(const QModelIndex & index){

	qDebug() << "Hola";

	dialogo = new DTablaModificar(listaUsuarios.at(index.row()));
	
	connect(dialogo, SIGNAL(senyalGuardar(Usuarios *)),
				this, SLOT (slotModificar(Usuarios*)));
	
	dialogo->show();

}



Qt::ItemFlags ModeloTabla::flags(const QModelIndex &index) const{

	return QAbstractTableModel::flags(index);

}


int ModeloTabla::rowCount(const QModelIndex &parent) const{
	
	return listaUsuarios.size();
}

QVariant ModeloTabla::data(const QModelIndex &index, int role) const{
	
	int fila = index.row();
	int columna = index.column();
	
	if (role == Qt::BackgroundColorRole)
		return QVariant();

	if (role != Qt::DisplayRole && columna == 0) 
		return QVariant();
	if (role != Qt::DisplayRole && columna == 1) 
		return QVariant();
	if (role != Qt::DisplayRole && columna == 2) 
		return QVariant();
		
	if (role == Qt::CheckStateRole && columna == 3) {
        // Devuelve el estado del checkbox basado en el valor booleano
        return listaUsuarios.at(fila)->is_premium ? Qt::Checked : Qt::Unchecked;
    }
		
	

	
	switch(columna){
		case 0:

			return QVariant(QString::number(listaUsuarios.at(fila)->uid));
			break;
			
		case 1:

			return QVariant(QString(listaUsuarios.at(fila)->login));
			break;
			
		case 2:
				 //qDebug() << "Karma " << usuarios.at(fila)->karma;
			return QVariant(QString::number(listaUsuarios.at(fila)->karma));
			break;
		
	}
	
	return QVariant();
}

int ModeloTabla::columnCount (const QModelIndex &parent) const{

	return 4;
}

QVariant ModeloTabla::headerData (int section, Qt::Orientation orientation, int role) const{

	QStringList lista = {"UID","Login","Karma","Es Premium"};

	if (role != Qt::DisplayRole) return QVariant();
	
	QString cadena;
	
	if (orientation == Qt::Horizontal){
		
		return QVariant(QString(lista.at(section)));
	}
	
	for (int i = 0; i < listaUsuarios.size(); i++){
		cadena = QString(listaUsuarios.at(section)->name);
		return QVariant(QString(cadena));
	}
	
}

void ModeloTabla::slotModificar(Usuarios* usu){

	QString databaseName = "Proyecto";
   QString password = "admin";
	
   QByteArray data = putData(databaseName,password,usu);
   
   QNetworkRequest request;
   request.setUrl(QUrl("http://192.168.1.37:8069/jsonrpc"));
   request.setRawHeader(
		QByteArray("Content-Type"),
		QByteArray("application/json"));


		
	if (usu->uid != 2){
		QNetworkReply *reply = manager->put(request, data);
	}
}

QByteArray ModeloTabla::putData(QString databaseName, QString password, Usuarios * usuario){

	 QJsonObject jsonObject;
    jsonObject.insert("jsonrpc", "2.0");
    jsonObject.insert("method", "call");
    
    QJsonObject params;
    params.insert("service", "object");
    params.insert("method", "execute");
    
    QJsonArray args;
    args.append(databaseName); // "jessica2dam" nom de la base de datos
    args.append(2);    // es el usuari
    args.append(password); // "1234" password
    args.append("res.users");
    args.append("write");
    args.append(usuario->uid);
    
    
    QJsonObject fields;
	 fields.insert("name", QString(usuario->name));
	 fields.insert("login", QString(usuario->login));
	 fields.insert("karma", QString::number(usuario->karma));
    args.append(fields);
    
    params.insert("args", args);
	jsonObject.insert("params", params);
	
   quint64 value64;
   value64 = QRandomGenerator().generate64();
   
	jsonObject.insert("id", QString::number(value64));
    
    QByteArray postData = QJsonDocument(jsonObject).toJson();
    qDebug().noquote()<< QString(postData);
    
	
	 temporizador->setInterval(1000);
	 temporizador->setSingleShot(true);
	 temporizador->start();

    return postData;
}



void ModeloTabla::eliminarUsuario(int uid){
	
	QString databaseName = "Proyecto";
   QString password = "admin";
	
   QByteArray data = deleteData(databaseName,password,uid);
   
   QNetworkRequest request;
   request.setUrl(QUrl("http://192.168.1.37:8069/jsonrpc"));
   request.setRawHeader(
		QByteArray("Content-Type"),
		QByteArray("application/json"));


	QNetworkReply *reply = manager->put(request, data);
	

}

QByteArray ModeloTabla::deleteData(QString databaseName, QString password, int uid){

	 QJsonObject jsonObject;
    jsonObject.insert("jsonrpc", "2.0");
    jsonObject.insert("method", "call");
    
    QJsonObject params;
    params.insert("service", "object");
    params.insert("method", "execute");
    
    QJsonArray args;
    args.append(databaseName); // "jessica2dam" nom de la base de datos
    args.append(2);    // es el usuari
    args.append(password); // "1234" password
    args.append("res.users");
    args.append("unlink");
    
    
    QJsonArray fields;
	 fields.append(uid);
    args.append(fields);
    
    params.insert("args", args);
	jsonObject.insert("params", params);
	
   quint64 value64;
   value64 = QRandomGenerator().generate64();
   
	jsonObject.insert("id", QString::number(value64));
    
    QByteArray postData = QJsonDocument(jsonObject).toJson();
    qDebug().noquote()<< QString(postData);
    
	
	 temporizador->setInterval(1000);
	 temporizador->setSingleShot(true);
	 temporizador->start();

    return postData;
}

void ModeloTabla::slotDialogoCrear(){

	DTablaCrear *dialogo = new DTablaCrear();
	
	connect(dialogo,SIGNAL(senyalCrear(Usuarios*)),
				this, SLOT(slotCrear(Usuarios*)));
	
	
	dialogo->show();

}

void ModeloTabla::slotCrear(Usuarios* usu){
	
	QString databaseName = "Proyecto";
   QString password = "admin";
	
   QByteArray data = postData(databaseName,password,usu);
   
   QNetworkRequest request;
   request.setUrl(QUrl("http://192.168.1.37:8069/jsonrpc"));
   request.setRawHeader(
		QByteArray("Content-Type"),
		QByteArray("application/json"));


	QNetworkReply *reply = manager->post(request, data);



}


QByteArray ModeloTabla::postData(QString databaseName, QString password, Usuarios * usuario){

	 QJsonObject jsonObject;
    jsonObject.insert("jsonrpc", "2.0");
    jsonObject.insert("method", "call");
    
    QJsonObject params;
    params.insert("service", "object");
    params.insert("method", "execute");
    
    QJsonArray args;
    args.append(databaseName); // "jessica2dam" nom de la base de datos
    args.append(2);    // es el usuari
    args.append(password); // "1234" password
    args.append("res.users");
    args.append("create");

    
    QJsonObject fields;
	 fields.insert("name", QString(usuario->name));
	 fields.insert("login", QString(usuario->login));
	 fields.insert("password", QString(usuario->pass));
	 fields.insert("karma", QString::number(usuario->karma));
    args.append(fields);
    
    params.insert("args", args);
	jsonObject.insert("params", params);
	
   quint64 value64;
   value64 = QRandomGenerator().generate64();
   
	jsonObject.insert("id", QString::number(value64));
    
    QByteArray postData = QJsonDocument(jsonObject).toJson();
    qDebug().noquote()<< QString(postData);
    
	
	 temporizador->setInterval(1000);
	 temporizador->setSingleShot(true);
	 temporizador->start();

    return postData;
}

















