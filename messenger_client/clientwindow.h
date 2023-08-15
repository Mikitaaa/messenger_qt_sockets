#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include "client.h"

class QLineEdit;
class QPushButton;

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void sendMessage();

private:
    Client *client;
    QLineEdit *messageLineEdit;
    QPushButton *sendButton;
};
#endif // CLIENTWINDOW_H
