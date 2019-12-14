#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QStandardItemModel>

namespace Ui {
class ServerWindow;
}

class QTcpServer;
class QTcpSocket;

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget* parent = 0);
    ~ServerWindow();

public slots:
    void slot_listen();

    void slot_stop();

    void slot_send();

private:
    Ui::ServerWindow* ui;
    QTcpServer* server;

    QStandardItemModel* tableModel;
    QStandardItemModel* listModel;

    QVector<QTcpSocket*> socketList;
    QMap<QTcpSocket*, QString> clientMap;

};

#endif // SERVERWINDOW_H
