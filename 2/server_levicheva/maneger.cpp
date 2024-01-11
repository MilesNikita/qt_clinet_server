#include "maneger.h"
#include "server.h"
#include "ui_maneger.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QHostAddress>
#include <QUdpSocket>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QVector>
#include <QtAlgorithms>
#include <QAxObject>
#include <QAxWidget>

QStringList list;
Maneger::Maneger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Maneger)
{
    ui->setupUi(this);
    tcp_c = new Server(ui->textEdit);
    QFile file("BD.sqlite");
    file.remove();
    server.initServer(505, ui->textEdit, ui->lineEdit, ui->lineEdit_2, ui->lineEdit_3);
    ui->lineEdit->repaint();
    ui->lineEdit_3->repaint();
    ui->lineEdit_2->repaint();
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &Maneger::save);
    pword = new QAxObject("Word.Application");
    pword->setProperty("Visible", true);
    QAxObject *pdoc = pword->querySubObject("Documents");
    pdoc = pdoc->querySubObject("Add()");
    QAxObject *pactive = pword->querySubObject("ActiveDocument()");
    prange = pactive->querySubObject("Range()");
    prange->setProperty("Text", "Данные для сохранения: \n");
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &Maneger::saveword);
}

Maneger::~Maneger()
{
    delete ui;
}

void Maneger::save()
{
    list.append(ui->lineEdit->text());
    static int i = 1;
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("BD.sqlite");
        if (!db.open()) {
                    qDebug() << "Cannot open database:" << db.lastError();
            }
        QSqlQuery query;
        QString   str  = "CREATE TABLE INFO ( "
                                     "id INTEGER PRIMARY KEY NOT NULL, "
                                     "put VARCHAR(1000),"
                                     "data_izme VARCHAR(100),"
                                     "data_dostupa VARCHAR(100)"
                                 ");";

        if (!query.exec(str)) {
                    qDebug() << "Unable to create a table";
                }
        QString strF =
                      "INSERT INTO  INFO (id, put, data_izme, data_dostupa) "
                      "VALUES(%1, '%2', '%3', '%4');";
        str = strF.arg(i)
                   .arg(ui->lineEdit->text())
                   .arg(ui->lineEdit_2->text())
                   .arg(ui->lineEdit_3->text());
        if (!query.exec(str)) {
                    qDebug() << "Unable to do insert opeation";
                }
        else
        {
             i++;
        }
        if (!query.exec("SELECT * FROM INFO;")) {
                    qDebug() << "Unable to execute query - exiting";
        }

}

void Maneger::analit()
{

}

void Maneger::saveword()
{
    prange->dynamicCall("InsertAfter(Text)", ui->lineEdit->text()+"\n");
}
