#ifndef MANEGER_H
#define MANEGER_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include "server.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QAxObject>

namespace Ui {
class Maneger;
}

class Maneger : public QWidget
{
    Q_OBJECT

public:
    explicit Maneger(QWidget *parent = nullptr);
    ~Maneger();

private slots:
    void analit();

    void saveword();

    void save();



private:
    Ui::Maneger *ui;
    Server server;
    Server *tcp_c;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
    QAxObject *pword;
    QAxObject *prange;
};

#endif // MANEGER_H
