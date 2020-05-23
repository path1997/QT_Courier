#ifndef MONITORINGPACZEK_H
#define MONITORINGPACZEK_H

#include "qjsontablemodel.h"
#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QDebug>
#include <QFileInfo>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QVariant>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QtNetwork/QNetworkRequest>

QT_BEGIN_NAMESPACE
namespace Ui { class MonitoringPaczek; }
QT_END_NAMESPACE

class MonitoringPaczek : public QMainWindow
{
    Q_OBJECT

public:
    MonitoringPaczek(QWidget *parent = nullptr);
    ~MonitoringPaczek();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MonitoringPaczek *ui;
    QSqlDatabase mydb;
    int iloscPrzesylek;
    int licznik;
    bool przycisk;
    QStringList listaPrzesylek;
    void getPp(int numerPrzesylki,QSqlQuery* qry,QSqlQueryModel * modal);
    void getDhl(int numerPrzesylki,QSqlQuery* qry,QSqlQueryModel * modal);
    void getInpost(int numerPrzesylki,QSqlQuery* qry,QSqlQueryModel * modal);

};
#endif // MONITORINGPACZEK_H
