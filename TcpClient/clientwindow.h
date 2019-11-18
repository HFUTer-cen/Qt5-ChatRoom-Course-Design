#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class ClientWindow;
}

class QTcpSocket;

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = 0);
    ~ClientWindow();

public slots:
    void slot_connect();

    void slot_disconnect();

    void slot_sendMessage();

    void slot_recvMessage();

private:
    Ui::ClientWindow* ui;
    QTcpSocket* socket;

};

#endif // CLIENTWINDOW_H
