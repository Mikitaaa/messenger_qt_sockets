#include "serverwindow.h"
#include <QPushButton>

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
{
    server = new Server(this);
    startButton = new QPushButton("Start Server", this);
    stopButton = new QPushButton("Stop Server", this);
    connect(startButton, &QPushButton::clicked, server, &Server::start);
    connect(startButton, &QPushButton::clicked, server, &Server::stop);
}

ServerWindow::~ServerWindow()
{
}

