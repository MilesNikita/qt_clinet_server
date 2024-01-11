#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();

    void skan();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    Client *tcp_c;
};
#endif // MAINWINDOW_H
