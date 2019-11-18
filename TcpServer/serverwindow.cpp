#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <QStringList>
#include <QTime>


ServerWindow::ServerWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Server Monitor");
    this->socketList = {};
    this->clientMap = {};
    this->server = new QTcpServer();

    /*
     *  设置表头和列宽
     */
    QStringList header = {"Socket ID", "IP Address", "Port", "Connected Time"};
    this->tableModel = new QStandardItemModel();
    this->tableModel->setHorizontalHeaderLabels(header);
    ui->tableView->setModel(this->tableModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QObject::connect(this->server, &QTcpServer::newConnection, [this](){
       QTcpSocket* socket = this->server->nextPendingConnection();

       QObject::connect(socket, &QTcpSocket::readyRead, [socket,this](){
           QString from = this->clientMap.value(socket);
           QString message = socket->readAll().data();

           for(int i = 0; i < this->socketList.size(); i++) {
               if(socket == socketList[i]) {
                   continue;
               }
               socketList[i]->write((from + " >> " + message).toUtf8());
           }
       });

       this->socketList.push_back(socket);
       this->clientMap.insert(socket, "client-" + QString::number(clientMap.count() + 1));

       QList<QStandardItem*> items = {
           new QStandardItem(QString::number(socket->socketDescriptor())),
           new QStandardItem(socket->peerAddress().toString()),
           new QStandardItem(QString::number(socket->peerPort())),
           new QStandardItem(QTime::currentTime().toString("hh:mm:ss"))
       };
       this->tableModel->appendRow(items);
    });

    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, [this](){
        ui->send_btn->setEnabled(ui->lineEdit->text() != "");
    });

    QObject::connect(ui->listen_btn, &QPushButton::clicked, this, &ServerWindow::slot_listen);

    QObject::connect(ui->stop_btn, &QPushButton::clicked, this, &ServerWindow::slot_stop);

    QObject::connect(ui->send_btn, &QPushButton::clicked, this, &ServerWindow::slot_send);
}

ServerWindow::~ServerWindow()
{
    delete ui;
    server->close();
    delete server;
}

void ServerWindow::slot_listen()
{
    this->server->listen(QHostAddress::LocalHost, 8000);
    ui->listen_btn->setEnabled(false);
    ui->stop_btn->setEnabled(true);
}


void ServerWindow::slot_stop()
{
    server->close();
    ui->listen_btn->setEnabled(true);
    ui->stop_btn->setEnabled(false);
}

void ServerWindow::slot_send()
{
    for(int i = 0; i < this->socketList.size(); i++) {
        socketList[i]->write(("Server >> " + ui->lineEdit->text()).toUtf8());
    }
    ui->lineEdit->setText("");

}
