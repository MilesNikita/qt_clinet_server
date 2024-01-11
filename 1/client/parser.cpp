#include "mainwindow.h"
#include "parser.h"

#define DIV 1024

Parser::Parser () {
}

QString MainWindow::JsonToString (QJsonObject *a)
{
    return QString (QJsonDocument (*a).toJson(QJsonDocument::Compact));
}

QJsonObject MainWindow::StringToJson (QString a)
{
    return QJsonDocument::fromJson (a.toUtf8()).object();
}

void Parser::slotGetSystemInfo ()
{
    QJsonObject obj;
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx (&statex);
    int total = statex.ullTotalPhys / DIV / DIV;
    int avail = statex.ullAvailPhys / DIV / DIV;
    int svob = (statex.ullTotalPhys - statex.ullAvailPhys) / DIV / DIV;
    obj["Full_memory_capacity"] = QString::number(total).replace('\\', '/').replace('//','/');
    obj["Occupied_amount_of_memory"] = QString::number(avail).replace('\\', '/').replace('//','/');
    obj["Free_memory_capacity"] = QString::number(svob).replace('\\', '/').replace('//','/');
    qDebug() << obj;
    emit getResult (obj);
}

