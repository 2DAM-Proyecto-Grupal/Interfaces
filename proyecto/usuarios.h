#ifndef _USUARIOS_H_
#define _USUARIOS_H_

#include <QString>

class Usuarios{
public:

	Usuarios();
	
	int uid;
	QString name;
	QString login;
	QString pass;
	bool is_premium;
	int karma;



};

#endif
