#ifndef MANEGER_H
#define MANEGER_H

#include <QWidget>
#include <QRegExpValidator>
#include <QRegExp>
#include <QJsonArray>
#include <QFileDialog>
#include "server.h"
#include <QAxObject>
#include <QMessageBox>
#include <QVariant>
#include <QtAlgorithms>
#include <QtMath>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTimer>
#include <QThread>

namespace Ui {
class Maneger;
}

class Maneger : public QWidget
{
    Q_OBJECT

public:
    explicit Maneger(QWidget *parent = nullptr);
    ~Maneger();
    bool createDataBase (QString path);
    Server *server;
    QString ip;
    QPair<QString, int> curAg;
    QVector<QPair<QString, int>> agents;

private slots:
    QString JsonToString (QJsonObject *a);
    QJsonObject StringToJson (QString a);
    void appendline (QJsonObject &, QPair<QString, int> &);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

public slots:
    void slotSendRequest (bool);
    void slotAgentConnect (QPair<QString, int>);
    void slotAgentDisconnect (QPair<QString, int>);
    void slotOperationFinish (bool, QPair<QString, int>);
    void slotProcess (QPair<QString, int>, QJsonObject);
    void skolokno();
    void timer();
    void starte();

private:
    Ui::Maneger *ui;
    QTimer *t;
    QAxObject *excel;
    QAxObject *workbooks;
    QAxObject *workbook;
    QAxObject *sheets;
    QAxObject *good;

signals:
    void makeConnection (QPair<QString, int>);
    void sendMessage (QString, QPair<QString, int>);

};
#endif // MAINWINDOW_H
