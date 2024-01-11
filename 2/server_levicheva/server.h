#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QMessageBox>
#include <QTextEdit>
#include <QTableWidget>
#include <QLineEdit>
#include <QTimer>

class Server : public QWidget
{
        Q_OBJECT
    public:
        explicit Server (QWidget *parent = nullptr);
        QTcpServer *server;
        int port;
        quint16 block;
        QTextEdit *te;
        QLineEdit *table;
        QLineEdit *table1;
        QLineEdit *table2;

    signals:

    public slots:
        bool initServer (int, QTextEdit *, QLineEdit*, QLineEdit *, QLineEdit *);
        void slotNewConnection();
        QString slotReadClient();
        void sendToClient (QTcpSocket *, const QString &);
};

#endif // SERVER_H
