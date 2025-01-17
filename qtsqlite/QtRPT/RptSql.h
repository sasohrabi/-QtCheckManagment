/*
Name: QtRpt
Version: 1.5.0
Programmer: Aleksey Osipov
e-mail: aliks-os@ukr.net
2012-2015
*/

#ifndef RPTSQL_H
#define RPTSQL_H

#include <QObject>
#include <QSqlQuery>

class RptSql : public QObject
{
    Q_OBJECT
public:
    explicit RptSql(QString dbType, QString dbName, QString dbHost, QString dbUser, QString dbPassword, int dbPort, QString dbConnectionName, QObject *parent = 0);
    bool openQuery(QString sql, QString dbCoding, QString charsetCoding);
    int getRecordCount();
    QString getFieldValue(QString fieldName, int recNo);

private:
    QSqlDatabase db;
    QSqlQuery *query;

signals:

public slots:

};

#endif // RPTSQL_H
