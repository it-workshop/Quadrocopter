#ifndef DB_SAVER_H
#define DB_SAVER_H

#include <QtSql>
#include "vect.h"

#define DB_DRIVER "QPSQL"
#define DB_HOST "127.0.0.1"
#define DB_DBNAME "control_center"
#define DB_USER "control_center"
#define DB_PASSWD "123456789"

class db_saver
{
    QSqlDatabase myDB;
public:
    void connect();
    db_saver();
    void update_info(double throttle, double voltage, vect angle);
};

#endif // DB_SAVER_H
