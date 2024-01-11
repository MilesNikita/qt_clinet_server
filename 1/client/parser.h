#ifndef PARSER_H
#define PARSER_H
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QJsonDocument>
#include <QTimer>
#include <windows.h>
#include <psapi.h>
#include <intrin.h>
#include <conio.h>
#include <fileapi.h>
#include <tchar.h>
#include <QObject>
#include <QThread>

class Parser : public QObject
{
        Q_OBJECT

    public:
        Parser ();


    public slots:
        void slotGetSystemInfo();


    signals:
        void getResult (QJsonObject);

};

#endif // PARSER_H
