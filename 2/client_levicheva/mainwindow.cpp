#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QFileInfo>

QString put;
QStringList list1;
QVector<QDateTime> list1info, list1proinfo;
QStringList derect1, dirinfo, dirproinfo;
QString str1, str2, str3;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    QStringList info;
    info << tr("Найденные файлы") << tr("Дата изменения") << tr("Дата последнего доступа");
    ui->tableWidget->setHorizontalHeaderLabels(info);
    ui -> tableWidget -> horizontalHeader() -> setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 600);
    ui->tableWidget->setColumnWidth(1, 100);
    ui -> tableWidget -> verticalHeader() -> setDefaultSectionSize(20);
    ui -> tableWidget -> setFrameShape(QFrame::NoFrame);
    ui -> tableWidget -> setShowGrid(true);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                             + "\\." + ipRange
                             + "\\." + ipRange
                             + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEdit_2->setValidator(ipValidator);
    tcp_c = new Client (ui->textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()
{
    list1.clear();
    derect1.clear();
    dirinfo.clear();
    dirproinfo.clear();
    list1info.clear();
    list1proinfo.clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clear();
    QStringList list;
    list << tr("Найденные файлы") << tr("Дата изменения") << tr("Дата последнего доступа");
    ui->tableWidget->setHorizontalHeaderLabels(list);
    QString str = QFileDialog::getExistingDirectory(0,"Выберите директорию: ", "C:/");
    put = str;
    if (!str.isEmpty())
    {
        QDir dir(str);
        QProcess *LilPeep = new QProcess(this);
        QThread *XXXTentacion = new QThread(this);
        connect(LilPeep, SIGNAL(readyReadStandardOutput()),this, SLOT(skan()));
        connect(XXXTentacion, SIGNAL(started()), this, SLOT(skan()));
        LilPeep->start();
        XXXTentacion->start();
    }

}

void MainWindow::skan()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clear();
    QStringList list;
    list << tr("Найденные файлы") << tr("Дата изменения") << tr("Дата последнего доступа");
    ui->tableWidget->setHorizontalHeaderLabels(list);
    QDirIterator it(put, QStringList() << "*.*", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString pot = it.next();
        list1 << pot;
        QFileInfo file1(pot);
        list1info.append(file1.lastModified());
        list1proinfo.append(file1.lastRead());
    }
    for (int i = 0; i < list1.length();i++)
    {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(list1[i]));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(list1info[i].toString()));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(list1proinfo[i].toString()));
    }
}

void MainWindow::on_pushButton_clicked()
{
    derect1.clear();
    dirinfo.clear();
    dirproinfo.clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clear();
    QStringList list;
    list << tr("Найденные файлы") << tr("Дата изменения") << tr("Дата последнего доступа");
    ui->tableWidget->setHorizontalHeaderLabels(list);
    QDateTime date1 = ui->dateTimeEdit->dateTime();
    QDateTime date2 = ui->dateTimeEdit_2->dateTime();
    for (int i = 0; i < list1.size(); i++)
    {
        if (((list1info[i] >= date1 ) && (list1info[i] <= date2)) || ((list1proinfo[i] >= date1) && (list1proinfo[i] <= date2)))
        {
            derect1 << list1[i];
            dirinfo << list1info[i].toString();
            dirproinfo << list1proinfo[i].toString();
        }
    }
    for (int i = 0; i < derect1.size(); i++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(derect1[i]));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(dirinfo[i]));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(dirproinfo[i]));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    tcp_c->doConnect (ui->lineEdit_2->text(), 505);
    ui->textEdit->append ("Отправлено: " + tcp_c -> send_to_server(str1, str2, str3));
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if (column == 0){
    str1 = ui->tableWidget->item(row, column)->text();
    str2 = ui->tableWidget->item(row,column+1)->text();
    str3 = ui->tableWidget->item(row,column+2)->text();
    }
    else if (column == 1)
    {
        str1 = ui->tableWidget->item(row, column-1)->text();
        str2 = ui->tableWidget->item(row,column)->text();
        str3 = ui->tableWidget->item(row,column+1)->text();
    }
    else if (column == 2)
    {
        str1 = ui->tableWidget->item(row, column-2)->text();
        str2 = ui->tableWidget->item(row,column-1)->text();
        str3 = ui->tableWidget->item(row,column)->text();
    }
}
