#include "maneger.h"
#include "ui_maneger.h"

int ogr;
QVector<int> meme;
int tim;
Maneger::Maneger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Maneger)
{
    ui->setupUi(this);
    server = new Server();
    connect (server, &Server::AgentDisconnected, this, &Maneger::slotAgentDisconnect);
    connect (server, &Server::AgentConnected, this, &Maneger::slotAgentConnect);
    connect (server, &Server::operationFinished, this, &Maneger::slotOperationFinish);
    connect (this, &Maneger::sendMessage, server, &Server::sendToClient);
    connect (server, &Server::recievedMessage, this, &Maneger::slotProcess);
    connect (this, &Maneger::makeConnection, server, &Server::slotMakeConnection);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEdit_6->setValidator(ipValidator);
    ui->lineEdit_7->setValidator(new QIntValidator (1000, 99999, this));
    QFile file("BD.sqlite");
    file.remove();
    excel = new QAxObject ("Excel.Application");
    excel->setProperty ("Visible", true);
    excel->setProperty ("DisplayAlerts", "0");
    workbooks = excel->querySubObject ("Workbooks");
    workbook = workbooks->querySubObject ("Add()");
    sheets = workbook->querySubObject ("Sheets");
    good = sheets->querySubObject ("Item(int)", 1);
    good->setProperty ("Name", "Сохранение");
    QString add = "";
    good->querySubObject ("Cells(QVariant,QVariant)", 1, 1)->setProperty("Value", QVariant ("IP-адрес"));
    good->querySubObject ("Cells(QVariant,QVariant)", 1, 2)->setProperty("Value", QVariant("Используемая память"));
    good->querySubObject ("Cells(QVariant,QVariant)", 1, 3)->setProperty("Value", QVariant("Заданное ограничение"));
    good->querySubObject ("Cells(QVariant,QVariant)", 1, 4)->setProperty("Value", QVariant("Сообщение о привышение"));
    good->querySubObject ("Cells(QVariant,QVariant)", 1, 5)->setProperty("Value", QVariant("Время"));
}

Maneger::~Maneger()
{
    delete ui;
}


void Maneger::slotSendRequest (bool result)
{
    if (result) {
        QMessageBox::about (this, "", "Запрос отправлен агенту.");
    } else {
        QMessageBox::about (this, "", "Не удалось отправить запрос агенту.");
    }
}

void Maneger::slotAgentConnect (QPair<QString, int> agent)
{
    agents.append (agent);
}

void Maneger::slotAgentDisconnect (QPair<QString, int> agent)
{
    for (int i = 0; i < agents.size(); ++i) {
        if (agents[i] == agent) {
            agents.erase (agents.begin() + i);
            break;
        }
    }
}

void Maneger::slotOperationFinish (bool res, QPair<QString, int>)
{
    if (!res) {
        QMessageBox::about (this, "", "Операция не выполнена.");

    }
}

void Maneger::on_pushButton_clicked()
{
    QString ip = ui->lineEdit_6->text();
    int port = -1;
    try {
        port = ui->lineEdit_7->text().toInt();
    } catch (...) {
        QMessageBox::about (this, "", "Такого порта не существует!");
        return;
    }
    auto pair = qMakePair (ip, port);
    emit makeConnection (pair);
    QJsonObject obj;
    obj["request"] = "scan";
    emit sendMessage(JsonToString(&obj), pair);
    QTimer *timere = new QTimer(this);
    QThread *calc = new QThread(this);
    connect(calc, SIGNAL(started()), this, SLOT(starte()));
    connect(timere, SIGNAL(timeout()), this, SLOT(starte()));
    calc->start();
    timere->start(50);
}

void Maneger::starte(){
    QString ip = ui->lineEdit_6->text();
    int port = ui->lineEdit_7->text().toInt();
    auto pair = qMakePair (ip, port);
    emit makeConnection (pair);
    QJsonObject obj;
    obj["request"] = "scan";
    emit sendMessage(JsonToString(&obj), pair);
    timer();
}

void Maneger::slotProcess (QPair<QString, int> agent, QJsonObject reply)
{
    qDebug() << reply;
    if (reply.contains ("Full_memory_capacity")) {
            appendline(reply, agent);
    }

}

void Maneger::skolokno()
{
    if (meme.size() == 1)
        ui->lineEdit_4->setText(QString::number(meme.at(0)));
    else if (meme.size() > 1){
        int k = meme.size() / 2;
        long long sum = 0;
        for (int i = 0; i < k; i++)
            sum += meme[i];

        int res = sum;
        for (int i = k; i < meme.size(); i++)
        {
            sum += meme[i] - meme[i - k];
            res = fmaxl(sum, res);
        }
        ui->lineEdit_4->setText(QString::number(res/k));
    }
}

void Maneger::timer()
{
    if (ui->lineEdit->text() == "True")
    {
        tim ++;
    }
    else
    {
        tim = 0;
    }
}

void Maneger::appendline (QJsonObject &obj, QPair<QString, int> &agent)
{
    qDebug() << obj;
    ui->lineEdit_5->setText(agent.first);
    ui->lineEdit_9->setText(obj["Full_memory_capacity"].toString());
    ui->lineEdit_10->setText(obj["Occupied_amount_of_memory"].toString());
    meme.append(ui->lineEdit_2->text().toInt());
    ui->lineEdit_2->setText(obj["Free_memory_capacity"].toString());
    skolokno();
};

QString Maneger::JsonToString (QJsonObject *a)
{
    return QString (QJsonDocument (*a).toJson (QJsonDocument::Compact));
}

QJsonObject Maneger::StringToJson (QString a)
{
    return QJsonDocument::fromJson (a.toUtf8()).object();
}

void Maneger::on_pushButton_2_clicked()
{
    if (ui->lineEdit_5->text()!="")
    {
        ogr = ui->lineEdit_8->text().toInt();
        ui->lineEdit_3->setText(QString::number(ogr));
        if (ui->lineEdit_2->text().toInt() < ogr){
            ui->lineEdit->setText("False");}
        else{
            ui->lineEdit->setText("True");}
    }
}

void Maneger::on_pushButton_3_clicked()
{
    static int i = 1;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("BD.sqlite");
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError();
    }
    QSqlQuery query;
    QString   str  = "CREATE TABLE INFO ("
                  "number INTEGER PRIMARY KEY NOT NULL, "
                  "ip VARCHAR(20),"
                  "memory   INTEGER, "
                  "eror  VARCHAR(20), "
                  "time  INTEGER "
                  ");";

    if (!query.exec(str)) {
        qDebug() << "Unable to create a table";
    }
    QString strF =
        "INSERT INTO  INFO (number, ip, memory, eror, time) "
        "VALUES(%1, '%2', %3, '%4', %5);";
    str = strF.arg(i)
              .arg(ui->lineEdit_5->text())
              .arg(ui->lineEdit_2->text().toInt())
              .arg(ui->lineEdit->text())
              .arg(tim);
    if (!query.exec(str)) {
        qDebug() << "Unable to do insert opeation";
    }
    else
    {
        i++;
    }
    if (!query.exec("SELECT * FROM INFO;")) {
        qDebug() << "Unable to execute query - exiting";
    }
}

void Maneger::on_pushButton_4_clicked()
{
    int rowGood = 2;
    good->querySubObject ("Cells(QVariant,QVariant)", rowGood, 1)->setProperty("Value", QVariant (ui->lineEdit_5->text()));
    good->querySubObject ("Cells(QVariant,QVariant)", rowGood, 2)->setProperty("Value", QVariant(ui->lineEdit_2->text()));
    good->querySubObject ("Cells(QVariant,QVariant)", rowGood, 3)->setProperty("Value", QVariant(ui->lineEdit_3->text()));
    good->querySubObject ("Cells(QVariant,QVariant)", rowGood, 4)->setProperty("Value", QVariant(ui->lineEdit->text()));
    good->querySubObject ("Cells(QVariant,QVariant)", rowGood, 5)->setProperty("Value", QVariant(tim));
    rowGood++;

}
