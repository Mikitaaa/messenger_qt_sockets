#include "serverwindow.h"
#include <QtWidgets>

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent) {
    resize(450, 400);

    server = new Server(this);
    startButton = new QPushButton("Start Server", this);
    stopButton = new QPushButton("Stop Server", this);

    Serverlog = new QTextEdit(this);
    Serverlog->setReadOnly(true);

    connect(startButton, &QPushButton::clicked, server, &Server::start);
    connect(stopButton, &QPushButton::clicked, server, &Server::stop);
    connect(server, &Server::ThrowlogMessage, this, &ServerWindow::appendToLog);

    QGridLayout *layout = new QGridLayout;
        layout->addWidget(startButton, 0, 0);
        layout->addWidget(stopButton, 0, 1);
        layout->addWidget(Serverlog, 1, 0, 1, 2);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

ServerWindow::~ServerWindow() {}

void ServerWindow::appendToLog(const QString &Message) {
    Serverlog->append(Message + "<br>");
}

