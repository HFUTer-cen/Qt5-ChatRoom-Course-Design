#include "clientwindow.h"
#include "ui_clientwindow.h"

#include <QDateTime>
#include <QScrollBar>

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    setWindowTitle("Client");

    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    this->socket = new QTcpSocket(this);

    QObject::connect(ui->connect_btn, &QPushButton::clicked, this, &ClientWindow::slot_connect);

    QObject::connect(ui->disconnect_btn, &QPushButton::clicked, this, &ClientWindow::slot_disconnect);

    QObject::connect(ui->send_btn, &QPushButton::clicked, this, &ClientWindow::slot_sendMessage);

    QObject::connect(this->socket, &QTcpSocket::readyRead, this, &ClientWindow::slot_recvMessage);

    QObject::connect(ui->input_line, &QLineEdit::textChanged, this, [this](){
        ui->send_btn->setEnabled(ui->input_line->text() != "");
    });

}

ClientWindow::~ClientWindow()
{
    delete ui;
    delete socket;
}

void ClientWindow::slot_connect()
{
    QString address = ui->IP_line->text();
    int port = ui->Port_spin->text().toInt();

    this->socket->connectToHost(address, port);
    if (socket->waitForConnected(3000)) {
        ui->connect_btn->setEnabled(false);
        ui->disconnect_btn->setEnabled(true);
    }
}

void ClientWindow::slot_disconnect()
{
    this->socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState || socket->waitForDisconnected(3000)) {
        ui->connect_btn->setEnabled(true);
        ui->disconnect_btn->setEnabled(false);
    }

}

void ClientWindow::slot_sendMessage()
{
    QString str = ui->input_line->text();

    ui->input_line->setText("");

    ui->textEdit->setAlignment(Qt::AlignRight);
    ui->textEdit->setTextColor(Qt::blue);
    ui->textEdit->setCurrentFont(QFont("Consolas",10));
    ui->textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    ui->textEdit->setTextColor(Qt::black);
    ui->textEdit->setCurrentFont(QFont("Consolas",11));
    ui->textEdit->append(str + "\n");

    QScrollBar* scrollbar = ui->textEdit->verticalScrollBar();
    if (scrollbar) {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }

    this->socket->write(str.toUtf8());
}

void ClientWindow::slot_recvMessage()
{
    QString str = socket->readAll().data();

    ui->textEdit->setAlignment(Qt::AlignLeft);
    ui->textEdit->setTextColor(Qt::blue);
    ui->textEdit->setCurrentFont(QFont("Consolas",10));
    ui->textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    ui->textEdit->setTextColor(Qt::black);
    ui->textEdit->setCurrentFont(QFont("Consolas",11));
    ui->textEdit->append(str+"\n");

    QScrollBar* scrollbar = ui->textEdit->verticalScrollBar();
    if (scrollbar) {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}




