#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include "server.h"

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private:
    Server *server;
};
#endif // SERVERWINDOW_H
