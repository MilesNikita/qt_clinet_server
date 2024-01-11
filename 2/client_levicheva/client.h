#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTime>
#include <QMessageBox>
#include <QDebug>
#include <QTextEdit>
#include <QLineEdit>

class Client : public QWidget
{
        Q_OBJECT
    public:
        Client (QTextEdit *, QWidget *parent = nullptr);
        quint16 block;
        QTcpSocket *client;
        bool doConnect (QString host, int port);
        QTextEdit *te;
    signals:

    public slots:
        void slotConnected();
        QString slotReadyRead (bool);
        void slotError (QAbstractSocket::SocketError);
        QString send_to_server (QString, QString, QString);
};

#endif // CLIENT_H
