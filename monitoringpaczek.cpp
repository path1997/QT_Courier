#include "monitoringpaczek.h"
#include "ui_monitoringpaczek.h"

MonitoringPaczek::MonitoringPaczek(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MonitoringPaczek)
{
    ui->setupUi(this);
    licznik=0;
    przycisk=false;
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("C:/Users/Patryk/Documents/Qt/MonitoringPaczek/kurierzy.db");

    /*if(!mydb.open()){
        ui->label->setText("Nie udalo sie");
    } else {
        ui->label->setText("Polaczylo");
    }*/
}



MonitoringPaczek::~MonitoringPaczek()
{
    mydb.close();
    delete ui;
}

void MonitoringPaczek::getPp(int numerPrzesylki, QSqlQuery *qry, QSqlQueryModel *modal){
    QSqlQuery query;
    QString idprzesylki;
    if(query.exec("select * from PpInfo where nrPrzesylki='"+listaPrzesylek.at(numerPrzesylki)+"'")){
            if(query.next()){
                QString masa = query.value("masa").toString();
                QString gabaryt = query.value("gabaryt").toString();
                QString dataNadania = query.value("dataNadania").toString();
                QString rodzajDostawy = query.value("rodzajDostawy").toString();
                idprzesylki= query.value("id").toString();
                ui->textEdit->append("Masa: "+masa);
                ui->textEdit->append("Gabaryt: "+gabaryt);
                ui->textEdit->append("Data nadania: "+dataNadania);
                ui->textEdit->append("Rodzaj dostawy: "+rodzajDostawy);
            } else {
                ui->textEdit->append("Nie znaleziono przesyłki");
            }
        }
        qry->prepare("select data,miejsce,nazwa from PpTracking where przesylkaId='"+idprzesylki+"' order by id desc");
        qry->exec();
        modal->setQuery(*qry);
        ui->tableView->setModel(modal);
}

void MonitoringPaczek::getDhl(int numerPrzesylki, QSqlQuery *qry, QSqlQueryModel *modal){
    QSqlQuery query;
    QString idprzesylki;
    if(query.exec("select * from DhlInfo where nrPrzesylki='"+listaPrzesylek.at(numerPrzesylki)+"'")){
            if(query.next()){
                QString masa = query.value("masa").toString();
                QString odKogo = query.value("odKogo").toString();
                QString dataNadania = query.value("dataNadania").toString();
                QString kwotaZaPobraniem = query.value("kwotaZaPobraniem").toString();
                idprzesylki= query.value("id").toString();
                ui->textEdit->append("Masa: "+masa);
                ui->textEdit->append("Od: "+odKogo);
                ui->textEdit->append("Data nadania: "+dataNadania);
                ui->textEdit->append("Kwota za pobraniem: "+kwotaZaPobraniem);
            } else {
                ui->textEdit->append("Nie znaleziono przesyłki");
            }
        }
        qry->prepare("select data,miejsce,nazwa from DhlTracking where przesylkaId='"+idprzesylki+"' order by id desc");
        qry->exec();
        modal->setQuery(*qry);
        ui->tableView->setModel(modal);
}

void MonitoringPaczek::getInpost(int numerPrzesylki, QSqlQuery *qry, QSqlQueryModel *modal){
    QSqlQuery query;
    QString idprzesylki;



    if(query.exec("select * from InpostInfo where nrPrzesylki='"+listaPrzesylek.at(numerPrzesylki)+"'")){
            if(query.next()){
                QString masa = query.value("masa").toString();
                QString usluga = query.value("usluga").toString();
                QString dataNadania = query.value("dataNadania").toString();
                QString kategoriaWysylki = query.value("kategoriaWysylki").toString();
                idprzesylki= query.value("id").toString();
                //ui->textEdit->append("Masa: "+masa);
                if(usluga==""){
                    ui->textEdit->append("Usluga: Brak");
                } else {
                   ui->textEdit->append("Usluga: "+usluga);
                }

                ui->textEdit->append("Data nadania: "+dataNadania);
                ui->textEdit->append("Kategoria wysyłki: "+kategoriaWysylki);

                qry->prepare("select data,nazwa from InpostTracking where przesylkaId='"+idprzesylki+"'");
                qry->exec();
                modal->setQuery(*qry);
                ui->tableView->setModel(modal);
            } else {
                /*QNetworkAccessManager *nam = new QNetworkAccessManager(this);
                QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(serviceRequestFinished(QNetworkReply*)));
                QString snumer=QString::number(numerPrzesylki);
                QUrl url("https://api-shipx-pl.easypack24.net/v1/tracking/"+snumer);
                QNetworkReply* reply = nam->get(QNetworkRequest(url));
                if(reply->error() == QNetworkReply::NoError) {
                QString strReply = (QString)reply->readAll();

                       qDebug() << strReply;
                  }*/

                QEventLoop eventLoop;
                //qDebug()<<snumer;
                    // "quit()" the event-loop, when the network request "finished()"
                    QNetworkAccessManager mgr;
                    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

                    // the HTTP request
                    QNetworkRequest req( QUrl( QString("https://api-shipx-pl.easypack24.net/v1/tracking/"+listaPrzesylek.at(numerPrzesylki)) ) );
                    QNetworkReply *reply = mgr.get(req);
                    eventLoop.exec(); // blocks stack until "finished()" has been called

                    if (reply->error() == QNetworkReply::NoError) {

                        QString strReply = (QString)reply->readAll();

                        //parse json
                        qDebug() << "Response:" << strReply;
                        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

                        QJsonObject jsonObj = jsonResponse.object();

                        QJsonValue value = jsonObj.value("tracking_details");
                        QJsonArray array = value.toArray();
                        /*qDebug() << "service:" << jsonObj["service"].toString();
                        qDebug() << "status:" << jsonObj["status"].toString();*/
                        QString usluga= jsonObj["custom_attributes"].toString();
                        QString dataNadania= jsonObj["created_at"].toString();
                        dataNadania.replace(10,1," ");
                        dataNadania=dataNadania.left(19);
                        QString kategoriaWysylyki=jsonObj["service"].toString();
                        QString qwe="insert into InpostInfo (dataNadania, kategoriaWysylki, usluga, nrPrzesylki) values('"+dataNadania+"','"+kategoriaWysylyki+"','"+usluga+"','"+listaPrzesylek.at(numerPrzesylki)+"');";
                        qry->prepare(qwe);
                        qDebug()<<qwe;
                        qry->exec();
                        QString id;
                        if(query.exec("select id from InpostInfo where nrPrzesylki='"+listaPrzesylek.at(numerPrzesylki)+"'")){
                                if(query.next()){
                                    id = query.value("id").toString();
                                }
                        }



                        /*QJsonTableModel::Header header;
                        header.push_back( QJsonTableModel::Heading( { {"title","Title"},    {"index","title"} }) );
                        header.push_back( QJsonTableModel::Heading( { {"title","Season"},   {"index","season"} }) );
                        header.push_back( QJsonTableModel::Heading( { {"title","Episode"},  {"index","episode"} }) );*/
                       // ui->tableView->setModel(array);
                         //qDebug() << "array:" << array;
                         foreach (const QJsonValue & v, array){
                                 QString status= v.toObject().value("status").toString();
                                 QString datetime= v.toObject().value("datetime").toString();
                                 if( datetime.at( 10 ) == 'T' ){
                                     datetime.replace(10,1," ");
                                     datetime=datetime.left(19);
                                 }
                                 qry->prepare("insert into InpostTracking (nazwa, data, przesylkaId) values('"+status+"','"+datetime+"','"+id+"');");
                                 qry->exec();


                         }

                        delete reply;
                        getInpost(listaPrzesylek.at(numerPrzesylki).toInt(),qry,modal);
                        return;
                    }
                    else {
                        //failure
                        qDebug() << "Failure" <<reply->errorString();
                        delete reply;
                    }

                ui->textEdit->append("Nie znaleziono przesyłki");
            }
        }

}


void MonitoringPaczek::on_pushButton_clicked()
{
    if(!ui->numeryPrzesylek->toPlainText().isEmpty() || ui->czytanieZpliku->isChecked()){
    QSqlQueryModel * modal=new QSqlQueryModel();
    QSqlQuery* qry=new QSqlQuery(mydb);
    if(przycisk==false){
        przycisk=true;
        ui->pushButton->setText("Stop");
        ui->radioPP->setEnabled(false);
        ui->radioINPOST->setEnabled(false);
        ui->radioDHL->setEnabled(false);
        ui->numeryPrzesylek->setEnabled(false);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->czytanieZpliku->setEnabled(false);

    ui->textEdit->setText("");


    if(ui->czytanieZpliku->isChecked()){
        QFile File("C:/Users/Patryk/Documents/Qt/MonitoringPaczek/NumeryPaczek.txt");
        if(!File.open(QIODevice::ReadOnly))
        {
            qDebug("Error");
        } else {
            while(!File.atEnd()){
            listaPrzesylek.append(File.readLine().trimmed());
            }
        }
            File.close();
    } else {
    QString data=ui->numeryPrzesylek->toPlainText();
    listaPrzesylek = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    }
    iloscPrzesylek=listaPrzesylek.size();
    ui->iloscprzesylek->setText("Ilość podanych przesyłek: "+QString::number(iloscPrzesylek));
    ui->numerPrzesylki->setText("Przesylka: 1");

    if(!mydb.open()){
        qDebug()<<"Failed to open database";
    }
    QString idprzesylki;
    QSqlQuery query;

    if(ui->radioPP->isChecked()){
    ui->textEdit->append("Numer przesyłki: "+listaPrzesylek.at(0));
    getPp(0,qry,modal);

    } else if(ui->radioDHL->isChecked()){
        ui->textEdit->append("Numer przesyłki: "+listaPrzesylek.at(0));
        getDhl(0,qry,modal);


    } else if(ui->radioINPOST->isChecked()){

        ui->textEdit->append("Numer przesyłki: "+listaPrzesylek.at(0));
        getInpost(0,qry,modal);
    }
    } else {
        przycisk=false;
        ui->pushButton->setText("Sprawdź");
        listaPrzesylek.clear();
        ui->radioPP->setEnabled(true);
        licznik=0;
        ui->radioINPOST->setEnabled(true);
        ui->radioDHL->setEnabled(true);
        ui->numeryPrzesylek->setEnabled(true);
        ui->iloscprzesylek->setText("");
        ui->numerPrzesylki->setText("");
        ui->textEdit->setText("");
        modal->clear();
        ui->tableView->setModel(modal);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->czytanieZpliku->setEnabled(true);
    }} else {
        ui->textEdit->setText("Nie wprowadzono numeru przesyłki");
    }
}

void MonitoringPaczek::on_pushButton_2_clicked()
{
    if(licznik<listaPrzesylek.size()-1){

        licznik++;
        QString idprzesylki;
        QSqlQuery query;
        ui->numerPrzesylki->setText("Przesylka: "+QString::number(licznik+1));
    QSqlQueryModel * modal=new QSqlQueryModel();
    QSqlQuery* qry=new QSqlQuery(mydb);
    modal->clear();
    ui->tableView->setModel(modal);
    ui->textEdit->setText("");

    if(ui->radioPP->isChecked()){
    ui->textEdit->append("Numer przesyłki: "+listaPrzesylek.at(licznik));
    getPp(licznik,qry,modal);

    } else if(ui->radioDHL->isChecked()){
        ui->textEdit->append("Numer przesyłki: "+listaPrzesylek.at(licznik));
        getDhl(licznik,qry,modal);

    } else if(ui->radioINPOST->isChecked()){

        ui->textEdit->append("Numer przesyłki: "+listaPrzesylek.at(licznik));
        getInpost(licznik,qry,modal);

    }
    }
}

void MonitoringPaczek::on_pushButton_3_clicked()
{
    if(licznik>0){
        licznik--;
        QString idprzesylki;
        QSqlQuery query;
        ui->numerPrzesylki->setText("Przesylka: "+QString::number(licznik+1));
    QSqlQueryModel * modal=new QSqlQueryModel();
    QSqlQuery* qry=new QSqlQuery(mydb);
    modal->clear();
    ui->tableView->setModel(modal);
    ui->textEdit->setText("");


    if(ui->radioPP->isChecked()){
    ui->textEdit->append("Numer przesyłki: "+listaPrzesylek.at(licznik));
    getPp(licznik,qry,modal);


    } else if(ui->radioDHL->isChecked()){
        ui->textEdit->append("Numer przesyłki: "+listaPrzesylek.at(licznik));
        getDhl(licznik,qry,modal);


    } else if(ui->radioINPOST->isChecked()){

        ui->textEdit->append("Numer przesyłki: "+listaPrzesylek.at(licznik));
        getInpost(licznik,qry,modal);

    }
    }
}
