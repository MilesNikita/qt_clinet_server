#include "client.h"

Client::Client (QTextEdit *t, QWidget *parent) : QWidget (parent), te (t) {};

bool Client::doConnect (QString host, int port)
{
    block = 0;
    client = new QTcpSocket (this);
    client->connectToHost (host, port);
    connect (client, SIGNAL (connected()), SLOT (slotConnected()));
    connect (client, SIGNAL (readyRead()), SLOT (slotReadyRead()));
    connect (client, SIGNAL (error (QAbstractSocket::SocketError)), this, SLOT (slotError (QAbstractSocket::SocketError)));
}

QString Client::slotReadyRead (bool a = false)
{
    QString str = client->readAll();
    QString str1 = client->readAll();
    QDataStream in (client);
    QDataStream in1(client);
    in.setVersion (QDataStream::Qt_5_7);
    in1.setVersion(QDataStream::Qt_5_7);
    for (;;) {
        if (!block) {
            if (client->bytesAvailable() < sizeof (quint16)) if (!a && client->bytesAvailable() == 0) break;
            in >> block;
        }
        if (client->bytesAvailable() < block) if (!a && client->bytesAvailable() == 0) break;
        QString str;
        QString str1;
        in >> str;
        in1 >> str1;
        block = 0;
                te->append (str);
                qDebug() << str << "\n";
        return str;
    }
    return "";
}

void Client::slotError (QAbstractSocket::SocketError err)
{
    QString strError =
            "Ошибка: " + (err == QAbstractSocket::HostNotFoundError ?
                         "Хост не был найден." :
                         err == QAbstractSocket::RemoteHostClosedError ?
                         "Удаленный хост закрыт." :
                         err == QAbstractSocket::ConnectionRefusedError ?
                         "Отказ в соединении." :
                         QString (client->errorString())
                        );
        qDebug() << strError;
    te->append (strError);
    te->append (client->errorString());
        qDebug() << client->errorString();
    qDebug() << "\n";
}

QString Client::send_to_server (QString str1, QString str2, QString str3)
{
    QByteArray  arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);
    out.setVersion (QDataStream::Qt_5_7);
    out << quint16 (0) << str1 << str2 << str3;
    out.device()->seek (0);
    out << quint16 (arrBlock.size() - sizeof (quint16));
    client->write(arrBlock);
    return slotReadyRead (true);
}

void Client::slotConnected()
{
    te->append ("Получен сигнал connected()");
    qDebug() << "Получен сигнал connected()";
}
