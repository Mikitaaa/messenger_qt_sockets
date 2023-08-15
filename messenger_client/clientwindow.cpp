#include "clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent), client(new Client(this))
{
    messageLineEdit = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(messageLineEdit);
    layout->addWidget(sendButton);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    client->setServerPort(5000);
    client->setServerAddress("192.168.0.110");
    client->connectToServer();
}

ClientWindow::~ClientWindow()
{
    delete client;
}

void ClientWindow::sendMessage()
{
    QString message = messageLineEdit->text();
    client->sendMessage(message);
    messageLineEdit->clear();
}
