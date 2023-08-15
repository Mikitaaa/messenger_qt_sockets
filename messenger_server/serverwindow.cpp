#include "serverwindow.h"
#include <QPushButton>

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
{
    server = new Server(this);
    QPushButton *startButton = new QPushButton("Start Server", this);
    connect(startButton, &QPushButton::clicked, server, &Server::start);
}

ServerWindow::~ServerWindow()
{
}

