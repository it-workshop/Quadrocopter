#include "db_saver.h"
#include <assert.h>
#include <sstream>
#include <QDebug>

using std::stringstream;

void db_saver::connect()
{
    myDB = QSqlDatabase::addDatabase(DB_DRIVER);

    myDB.setHostName(DB_HOST);
    myDB.setDatabaseName(DB_DBNAME);
    myDB.setUserName(DB_USER);
    myDB.setPassword(DB_PASSWD);

    myDB.setConnectOptions("connect_timeout=15");

    if(!myDB.open())
        qDebug() << "DB ERROR";
    else qDebug() << "DB OK";
}

db_saver::db_saver()
{
    connect();
}

void db_saver::update_info(double throttle, double voltage, vect angle)
{
    QSqlQuery q1(myDB);
    q1.prepare("update copters set (pitch, yaw, roll, throttle, voltage, updated_at) = "
    "(:pitch, :yaw, :roll, :throttle, :voltage, current_timestamp)");

    myDB.transaction();
    q1.bindValue(":yaw", (double) angle.x);
    q1.bindValue(":pitch", (double) angle.y);
    q1.bindValue(":roll", (double) angle.z);
    q1.bindValue(":throttle", throttle);
    q1.bindValue(":voltage", voltage);
    q1.exec();

    qDebug() << "Rows affected: " << q1.numRowsAffected() << " Error text: " << q1.lastError().text();

    myDB.commit();
}
