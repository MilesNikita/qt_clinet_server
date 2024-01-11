#include "server.h"

Server::Server (QWidget *parent) : QWidget (parent) {}

bool Server::initServer (int port, QTextEdit *t, QLineEdit *tab, QLineEdit *tab1, QLineEdit *tab2)
{
    te = t;
    table = tab;
    table1 = tab1;
    table2 = tab2;
    server = new QTcpServer;
    if (!server->listen (QHostAddress::Any, port)) {
        QMessageBox::critical (nullptr,
                               "Ошибка сервера",
                               "Ошибка запуска сервера:"
                               + server->errorString());
        server->close();
        return false;
    }
    this->port = port;
    connect (server, SIGNAL (newConnection()), this, SLOT (slotNewConnection()));
    block = 0;
    return true;
};

void Server::slotNewConnection()
{
    QTcpSocket *sock = server->nextPendingConnection();

    connect (sock, SIGNAL (readyRead()),
             this, SLOT (slotReadClient()));

    connect (sock, SIGNAL (disconnected()),
             sock, SLOT (deleteLater()));

    sendToClient (sock, "Сервер подключен!");
};

QString Server::slotReadClient()
{
    QTcpSocket *sock = (QTcpSocket *)sender();
    QDataStream in (sock);
    QDataStream in1 (sock);
    QDataStream in2(sock);
    in.setVersion (QDataStream::Qt_5_7);
    while (true) {
        if (!block) {
            if (sock->bytesAvailable() < sizeof (quint16)) {
                break;
            }
            in >> block;
        }

        if (sock->bytesAvailable() <= block) {
            break;
        }
    }
    QString str, str1, str3;
    in >> str;
    in1 >> str1;
    in2 >> str3;
    QString strMess = "Клиент прислал путь к файлу: " + str;
    block = 0;
    te->append(strMess);
    table->setText(str);
    table1->setText(str1);
    table2->setText(str3);
    sendToClient (sock, "Server received message!");
    return strMess;

}

void Server::sendToClient (QTcpSocket *sock, const QString &str)
{
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);
    out.setVersion (QDataStream::Qt_5_7);
    out << quint16 (0) << str;
    out.device()->seek (0);
    out << (quint16 (arrBlock.size()) - sizeof (quint16));
    sock->write (arrBlock);
}
